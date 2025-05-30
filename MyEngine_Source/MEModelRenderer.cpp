#include "MEModelRenderer.h"
#include "MESceneManager.h"
#include "MEMesh.h"
#include "MECamera.h"
#include "MERenderer.h"
#include "METransform.h"
#include "MEMaterial.h"

namespace ME
{
ModelRenderer::ModelRenderer()
    :Component(enums::eComponentType::ModelRenderer)
{
	mMeshes.clear();
}
ModelRenderer::~ModelRenderer()
{

}

void ModelRenderer::Initialize()
{

}
void ModelRenderer::Update()
{

}
void ModelRenderer::LateUpdate()
{
   
}
void ModelRenderer::Render()
{
    render();
}

void ModelRenderer::setModelMatrix(const Matrix& model)
{
    mModelMatrix = model;
}
void ModelRenderer::SetMesh(const std::vector<Mesh*>& meshes_)
{
    mMeshes = meshes_;
}
void ModelRenderer::render()
{
 
    Transform* tr = GetOwner()->GetComponent<Transform>();

    if (tr)
        tr->Bind();


   if (mMaterial)
   {
       mMaterial->BindShader();
   }
   if (!mMeshes.empty())
   {
       for (auto mesh : mMeshes)
       {
           if (mesh == nullptr)
               continue;

           mesh->Bind();

           if(mesh->GetDiffuseTexture() != nullptr)
		   {
			   mesh->GetDiffuseTexture()->Bind(graphics::eShaderStage::PS, (UINT)graphics::eTextureType::Albedo);
               mbHasEmbeddedTextures = true;
           }

           if (mesh->GetSpecularTexture() != nullptr)
           {
               mesh->GetSpecularTexture()->Bind(graphics::eShaderStage::PS, (UINT)graphics::eTextureType::Specular);
               mbHasEmbeddedTextures = true;
           }


           if (mesh->GetNormalTexture() != nullptr)
           {
               mesh->GetNormalTexture()->Bind(graphics::eShaderStage::PS, (UINT)graphics::eTextureType::Normal);
               mbHasEmbeddedTextures = true;
           }
       }
   }

    //if (!mbHasEmbeddedTextures)
    //{
    //    for (auto texture : mTextures)
    //    {
    //        if (texture == nullptr)
    //            continue;
    //
    //        texture->Bind(graphics::eShaderStage::PS, (UINT)graphics::eTextureType::Albedo);
    //    }
    //}

    if (!mMeshes.empty())
    {
        for (auto mesh : mMeshes)
        {
            if (mesh == nullptr)
                continue;
  
            graphics::GetDevice()->DrawIndexed(mesh->GetIndexCount(), 0, 0);
        }
    }
       


}

}
