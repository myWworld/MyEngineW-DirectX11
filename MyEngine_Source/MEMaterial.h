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

		void SetShader(graphics::Shader* shader) { mShader = shader; }


	private:

		graphics::Shader* mShader;

		graphics::eRenderingMode mMode;
		Material::Data mData;

	};

}
