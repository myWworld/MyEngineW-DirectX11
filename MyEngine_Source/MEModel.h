#pragma once
#include "../MyEngine_Source/CommonInclude.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
namespace ME
{

    class Mesh;
    class Texture;

    class Model
    {
    public:
        Model();
        ~Model();

        bool LoadModel(const std::wstring& path);
        const std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return m_meshes; }

    private:

        void ProcessNode(aiNode* node, const aiScene* scene);
        std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::wstring& typeName);

    private:
        std::vector<std::shared_ptr<Mesh>> m_meshes;

    };

}