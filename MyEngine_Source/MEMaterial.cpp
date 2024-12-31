#include "MEMaterial.h"


namespace ME
{
	Material::Material()
		:Resource(enums::eResourceType::Material)
		, mMode(graphics::eRenderingMode::Opaque)
	{
	}
	Material::~Material()
	{
	}
	HRESULT Material::Save(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	HRESULT Material::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	void Material::Bind()
	{
		mShader->Bind();
	}
}