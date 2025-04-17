#pragma once
#include "../MyEngine_Source/CommonInclude.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "METexture.h"

namespace ME
{

    class Mesh;
  
    class Model
    {
    public:
        Model();
        ~Model();

        bool LoadModel(const std::wstring& path);
        const std::vector<Mesh*>& GetMeshes() { return mMeshes; }
        const std::vector<graphics::Texture*>&GetTextures() { return mTextures; }

    private:

        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<graphics::Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::wstring& typeName);

    private:
        std::vector<Mesh*> mMeshes;
        std::vector<graphics::Texture *> mTextures;

    };

}