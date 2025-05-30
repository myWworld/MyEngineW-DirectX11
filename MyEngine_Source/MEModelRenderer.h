#pragma once

#include "CommonInclude.h"
#include "MEComponent.h"
#include "METexture.h"


namespace ME
{

    class Mesh;
    class Material;
  

    class ModelRenderer : public Component
    {
    public:

        ModelRenderer();
        ~ModelRenderer();

        void Initialize() override;
        void Update() override;
        void LateUpdate() override;
        void Render() override;
    

        void setModelMatrix(const Matrix& model);
        void SetMesh(const std::vector<Mesh*>& meshes_);

        void SetTextures(const std::vector<graphics::Texture*>& textures) { mTextures = textures; }
        void SetTexture(graphics::Texture* texture) { mTextures.push_back(texture); }
        void SetMaterial(Material* material) { mMaterial = material; }

    private:
        void render();


    private:

        Matrix mModelMatrix;

        std::vector<Mesh*> mMeshes;
        std::vector<graphics::Texture* >mTextures;
        Material* mMaterial;

        bool mbHasEmbeddedTextures = false; // Flag to check if textures are embedded in the model

       
    };

}