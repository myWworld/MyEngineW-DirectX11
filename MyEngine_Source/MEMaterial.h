#pragma once
#include "MEResource.h"
#include "METexture.h"
#include "MEShader.h"


namespace ME
{
	class Material:public Resource
	{
	public:

		Material();
		virtual ~Material();

		struct Data 
		{
			std::wstring albedo;
		};

		virtual HRESULT Save(const std::wstring& path) override;
		virtual HRESULT Load(const std::wstring& path) override;

		void Bind();
		void BindShader();
		void BindTextures();

		void SetShader(graphics::Shader* shader) { mShader = shader; }
		void SetAlbedoTexture(graphics::Texture* texture) { mAlbedoTexture = texture; mData.albedo = texture->GetName(); }


	private:

		graphics::Texture* mAlbedoTexture;
		std::vector<graphics::Texture*> mAlbedoTextures;
		graphics::Texture* mNormalTexture;
		graphics::Texture* mSpecularTexture;


		graphics::Shader* mShader;

		graphics::eRenderingMode mMode;
		Material::Data mData;

	};

}
