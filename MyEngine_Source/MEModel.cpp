#include "MEModel.h"
#include "MEGraphics.h"
#include "METexture.h"
#include "MEMesh.h"


namespace ME
{
    using namespace graphics;
    using namespace math;

    Model::Model()
    {

    }

    Model::~Model()
    {
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
            m_meshes.push_back(ProcessMesh(mesh, scene));
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    std::shared_ptr<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<std::shared_ptr<ME::Texture>> textures;

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
          // aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
          // std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, //L"texture_diffuse");
          //
          // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
          //
          // std::vector<std::shared_ptr<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, //L"texture_specular");
          // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return std::make_shared<Mesh>(vertices, indices);
    }

   // std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::wstring& //typeName)
   // {
   //     std::vector<std::shared_ptr<Texture>> textures;
   //     for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
   //     {
   //         aiString str;
   //         mat->GetTexture(type, i, &str);
   //         Texture texture;
   //        
   //         texture.m_id = texture.LoadFromFile(str.C_Str());
   //         texture.m_type = typeName;
   //
   //         std::string path_str = str.C_Str();
   //         texture.m_path = std::wstring(path_str.begin(), path_str.end());
   //         textures.push_back(std::make_shared<Texture>(texture));
   //     }
   //     return textures;
   // }


}