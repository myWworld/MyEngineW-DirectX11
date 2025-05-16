#include "MEModel.h"
#include "MEGraphics.h"
#include "MEMesh.h"
#include "MERenderer.h"
#include "MEBone.h"


namespace ME
{
    using namespace graphics;
    using namespace math;

    Model::Model()
        :mModelType(enums::eModelType::Static)
    {}

    Model::~Model()
    {
        for (auto mesh : mMeshes)
        {
            if (mesh == nullptr)
                continue;
            delete mesh;
	        mesh = nullptr;
        }

        for(auto texture : mTextures)
		{
            if(texture == nullptr)
                continue;
			delete texture;
	        texture = nullptr;
		}
    }
    bool Model::LoadModel(const std::wstring& path)
    {
        Assimp::Importer importer;
        std::string path_str = std::string(path.begin(), path.end());
        importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

        auto preRot = LoadPreRotationsWithFBXSDK(path);

        const aiScene* scene = importer.ReadFile(path_str,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_RemoveRedundantMaterials | //  쓰레기 재질 제거
                    //  잘못된 데이터 자동 제거
          //  aiProcess_OptimizeMeshes |           //  메쉬 최적화
            aiProcess_ImproveCacheLocality   |   //  GPU 캐시 최적화
           // aiProcess_MakeLeftHanded |
            aiProcess_JoinIdenticalVertices |
            //aiProcess_FlipWindingOrder | //  좌표계 변환  
            aiProcessPreset_TargetRealtime_Quality
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string str = importer.GetErrorString();
    
            return false;
        }

       
        std::wstring directory = path.substr(0, path.find_last_of('/') + 1);
        
        mSkeleton.RegisterBone(scene->mRootNode, preRot);
        mSkeleton.BuildSkeleton(scene->mRootNode);
        
        ProcessNode(scene->mRootNode, scene);
    
        if (mModelType == enums::eModelType::SkinnedMesh)
        {
            mSkeleton.RegisterSkinData(scene);
        }

        return true;
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        bool hasSkinnedBone = false;


        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

            std::string meshName = mesh->mName.C_Str();      

            Mesh* mMesh = ProcessMesh(mesh, scene);

            if (mesh->mNumBones > 0)
            {
                hasSkinnedBone = true;
                renderer::LoadModels(mMesh);
                mMesh->SetSkinned(true);
            }
            else if (mesh->mNumBones == 0)
            {   
                renderer::LoadStaticModels(mMesh);   
                mMesh->SetSkinned(false);
            }
            
            mMeshes.push_back(mMesh);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }


        if(hasSkinnedBone && mbModelFixed == false)
		{
            mbModelFixed = true;
			mSkeleton.SetModelType(enums::eModelType::SkinnedMesh);
			mModelType = enums::eModelType::SkinnedMesh;
		}
		else if(hasSkinnedBone == false && mbModelFixed == false)
		{
			mSkeleton.SetModelType(enums::eModelType::Static);
			mModelType = enums::eModelType::Static;
		}
    }
    Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::unordered_map<std::string, int> boneNameToIndex;
        std::vector<Bone> bones; 

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            vertex.pos = Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

            if (mesh->HasNormals())
            {
                vertex.normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
            }

            if (mesh->HasTextureCoords(0))
            {
                vertex.uv = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            }

            if(mesh->HasVertexColors(0))
			{
                aiColor4D c = mesh->mColors[0][i];
				vertex.color = Vector4(c.r, c.g, c.b, c.a);
			}
			else
			{
				vertex.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			}

         
            vertices.push_back(vertex);

        }

        for (unsigned int i = 0; i < mesh->mNumBones; i++)
        {
            aiBone* bone = mesh->mBones[i];
            std::string boneName = bone->mName.C_Str();
           // boneName = mSkeleton.ExtractBoneName(boneName);

            int boneIndex = mSkeleton.GetBoneIndex(boneName);
            boneNameToIndex[boneName] = boneIndex;

            Bone newBone(boneName,boneIndex, math::Matrix::Identity);
            newBone.mOffsetMatrix = ConvertAIMatrixToMatrix(bone->mOffsetMatrix);
            bones.push_back(newBone);

            for (unsigned int j = 0; j < bone->mNumWeights; j++)
            {
                unsigned int vertexID = bone->mWeights[j].mVertexId;
                float weight = bone->mWeights[j].mWeight;

                vertices[vertexID].AddBoneData(boneIndex, weight);
                
            }
   
        }

        for (auto& vertex : vertices)
        {
            vertex.NormalizeBoneWeights();
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        Mesh* newMesh = new Mesh(vertices, indices,bones);

        if (mesh->mMaterialIndex >= 0)
        {
           aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
           std::vector<ME::Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, L"texture_diffuse");

           if(!diffuseMaps.empty())
               newMesh->SetDiffuseTexture(diffuseMaps[0]);
          
           mTextures.insert(mTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
          
           std::vector<ME::Texture*> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, L"texture_specular");
           mTextures.insert(mTextures.end(), specularMaps.begin(), specularMaps.end());
           if (!specularMaps.empty())
               newMesh->SetSpecularTexture(specularMaps[0]);

           std::vector<ME::Texture*> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, L"texture_normal");
          
           mTextures.insert(mTextures.end(), normalMaps.begin(), normalMaps.end());
           if (!normalMaps.empty())
               newMesh->SetNormalTexture(normalMaps[0]);
        }

     
        return newMesh;
    }

     std::vector<ME::Texture*> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::wstring& typeName)
     {
         std::vector<ME::Texture*> textures;
         for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
         {
             aiString str;
             mat->GetTexture(type, i, &str);
             Texture* texture = new Texture();
             const char* cstr = str.C_Str();
         	 
             std::string new_str(cstr);
             
             std::string texStr = new_str;
             
             if (texStr.find("..") == 0)
                 texStr = texStr.substr(3);

             std::filesystem::path base = std::filesystem::absolute(L"..\\Resources\\");
             std::filesystem::path fullPath = base / texStr;
        // OK!

          
             if (texture->Load(fullPath.wstring()) == S_OK)
             {
                 textures.push_back(texture);
             }
             else
             {
                 delete texture;
                 texture = nullptr;
             }
             
         }
         return textures;
    }

     std::unordered_map<std::string, math::Matrix> Model::LoadPreRotationsWithFBXSDK(const std::wstring& wpath)
     {
         std::unordered_map<std::string, math::Matrix> bonePreRotations;

         FbxManager* pManager = FbxManager::Create();
         FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
         pManager->SetIOSettings(ios);

         FbxImporter* pImporter = FbxImporter::Create(pManager, "");
         std::string path = std::string(wpath.begin(), wpath.end());

         FbxScene* pScene = FbxScene::Create(pManager, "TempScene");

         if (!pImporter->Initialize(path.c_str(), -1, pManager->GetIOSettings()) || !pImporter->Import(pScene)) {
             std::cerr << "FBX SDK Load Error: " << pImporter->GetStatus().GetErrorString() << std::endl;
             return bonePreRotations;
         }

         pImporter->Destroy();

         std::function<void(FbxNode*)> Traverse;
         Traverse = [&](FbxNode* node) {

             std::string nodeName = node->GetName();
             
             FbxVector4 preRot = node->GetPreRotation(FbxNode::eSourcePivot);
             
             FbxAMatrix localMat = node->EvaluateLocalTransform();

             Matrix local;
             for (int row = 0; row < 4; ++row)
                 for (int col = 0; col < 4; ++col)
                     (&local._11)[row * 4 + col] = static_cast<float>(localMat.Get(row, col));

             bonePreRotations[nodeName] = local;

             for (int i = 0; i < node->GetChildCount(); ++i)
                 Traverse(node->GetChild(i));
             };

         Traverse(pScene->GetRootNode());

         pManager->Destroy();
         return bonePreRotations;
     }

     math::Matrix Model::ConvertAIMatrixToMatrix(aiMatrix4x4& aiMat)
     {
      
         math::Matrix mat;

         mat._11 = aiMat.a1; mat._12 = aiMat.b1; mat._13 = aiMat.c1; mat._14 = aiMat.d1;
         mat._21 = aiMat.a2; mat._22 = aiMat.b2; mat._23 = aiMat.c2; mat._24 = aiMat.d2;
         mat._31 = aiMat.a3; mat._32 = aiMat.b3; mat._33 = aiMat.c3; mat._34 = aiMat.d3;
         mat._41 = aiMat.a4; mat._42 = aiMat.b4; mat._43 = aiMat.c4; mat._44 = aiMat.d4;

         return mat;
     }
}