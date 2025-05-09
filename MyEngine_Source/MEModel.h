#pragma once
#include "../MyEngine_Source/CommonInclude.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <fbxsdk.h>
#include <fbxsdk/scene/geometry/fbxnode.h>

#include "METexture.h"
#include "MESkeleton.h"

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
        Skeleton& GetSkeleton() { return mSkeleton; }

        enums::eModelType GetModelType() { return mModelType; }

    private:

        void ProcessNode(aiNode* node, const aiScene* scene);
        Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<graphics::Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::wstring& typeName);

        std::unordered_map<std::string, math::Matrix> LoadPreRotationsWithFBXSDK(const std::wstring& wpath);


    

        math::Matrix ConvertAIMatrixToMatrix(aiMatrix4x4& mat);

    private:
        std::vector<Mesh*> mMeshes;
        std::vector<graphics::Texture *> mTextures;
        Skeleton mSkeleton;

        enums::eModelType mModelType;

        bool mbModelFixed = false;

    };

}