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

        const aiScene* scene = importer.ReadFile(path_str,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_PreTransformVertices |   //  로컬 변환 미리 적용
            aiProcess_RemoveRedundantMaterials | //  쓰레기 재질 제거
            aiProcess_FindInvalidData |          //  잘못된 데이터 자동 제거
            aiProcess_OptimizeMeshes |           //  메쉬 최적화
            aiProcess_ImproveCacheLocality   |   //  GPU 캐시 최적화
            aiProcess_MakeLeftHanded |
            aiProcess_JoinIdenticalVertices |
            aiProcess_FlipWindingOrder  //  좌표계 변환
        );

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            return false;
        }

       
        std::wstring directory = path.substr(0, path.find_last_of('/') + 1);
        ProcessNode(scene->mRootNode, scene);
     
        mSkeleton.RegisterBone(scene->mRootNode);
        mSkeleton.BuildSkeleton(scene->mRootNode);
        return true;
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

            std::string meshName = mesh->mName.C_Str();

            if (meshName.find("Teeth") != std::string::npos ||
                meshName.find("EyeLeft") != std::string::npos ||
                meshName.find("EyeRight") != std::string::npos)
            {
                // 이 메쉬는 스킵 (입, 눈)
                continue;
            }
       
            

            Mesh* mMesh = ProcessMesh(mesh, scene);

            if (mesh->mNumBones > 0)
            {
                renderer::LoadModels(mMesh);
                mModelType = enums::eModelType::SkinnedMesh;
            }
            else if (mesh->mNumBones == 0)
            {
                renderer::LoadStaticModels(mMesh);
                mModelType = enums::eModelType::Static;
            }
            
            mMeshes.push_back(mMesh);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
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

            int boneIndex = bones.size();
            boneNameToIndex[boneName] = boneIndex;

            Bone newBone(boneName,boneIndex, math::Matrix::Identity);
            newBone.mOffsetMatrix = ConvertAIMatrixToMatrix( bone->mOffsetMatrix);
            bones.push_back(newBone);

            for (unsigned int j = 0; j < bone->mNumWeights; j++)
            {
                unsigned int vertexID = bone->mWeights[j].mVertexId;
                float weight = bone->mWeights[j].mWeight;

                vertices[vertexID].AddBoneData(boneIndex, weight);
            }
   
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
               newMesh->SetDiffuseTexture(specularMaps[0]);
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
             std::wstring path = std::wstring(new_str.begin(), new_str.end());
          
             if (texture->Load(path) == S_OK)
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