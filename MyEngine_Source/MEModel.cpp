#include "MEModel.h"
#include "MEGraphics.h"
#include "MEMesh.h"
#include "MERenderer.h"


namespace ME
{
    using namespace graphics;
    using namespace math;

    Model::Model()
    {

    }

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

        const aiScene* scene = importer.ReadFile(path_str, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            return false;
        }

        std::wstring directory = path.substr(0, path.find_last_of('/') + 1);
        ProcessNode(scene->mRootNode, scene);
        return true;
    }

    void Model::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            Mesh* mMesh = ProcessMesh(mesh, scene);
            renderer::LoadModels(mMesh);
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

        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0)
        {
           aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
           std::vector<ME::Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, L"texture_diffuse");
          
           mTextures.insert(mTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
          
           std::vector<ME::Texture*> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, L"texture_specular");
           mTextures.insert(mTextures.end(), specularMaps.begin(), specularMaps.end());
        }

        return new Mesh(vertices, indices);
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
          
             if (texture->Load(path))
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


}