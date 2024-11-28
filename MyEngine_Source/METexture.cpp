#include "METexture.h"
#include "MEApplication.h"
#include "MEResources.h"

extern ME::Application application;

namespace ME::graphics
{
	

	Texture::Texture()
		:Resource(enums::eResourceType::Texture)
		,mDesc{}
	{

	}

	Texture::~Texture()
	{
	}

	HRESULT Texture::Save(const std::wstring& path)
	{
		return S_OK;
	 }

	HRESULT Texture::Load(const std::wstring& path)
	{
		//bmp 일때
		std::wstring ext 
			= path.substr(path.find_last_of(L".") + 1);

		if (ext == L".dds" || ext == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(path.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
				return S_FALSE;
		}
		else if (ext == L".tga" || ext == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(path.c_str(), nullptr, mImage)))
				return S_FALSE;
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(LoadFromWICFile(path.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
				return S_FALSE;
		}

		HRESULT hr = CreateShaderResourceView
		(
			graphics::GetDevice()->GetID3D11Device().Get()
			, mImage.GetImages()
			, mImage.GetImageCount()
			, mImage.GetMetadata()
			, mSRV.GetAddressOf()
		);

		if (hr == S_FALSE)
			assert(false);

		mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());

		
	  //
		////png 일때
		//	
		//if (ext == L"png")
		//{
		//	mType = eTextureType::Png;
		//	mImage = Gdiplus::Image::FromFile(path.c_str());
	  //
		//	if (mImage == nullptr) { return S_FALSE; }
	  //
		//	mWidth = mImage->GetWidth();
		//	mHeight = mImage->GetHeight();
		//}
		//else if (ext == L"bmp")
		//{
		//	mType = eTextureType::Bmp;
	  //
		//	mBitmap = (HBITMAP)LoadImageW(nullptr, path.c_str(), IMAGE_BITMAP,
		//		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	  //
		//	if (mBitmap == nullptr) { return S_FALSE; }
	  //
		//	BITMAP info = {};
		//	GetObject(mBitmap, sizeof(BITMAP), &info);
	  //
		//	mWidth = info.bmWidth;
		//	mHeight = info.bmHeight;
	  //
		//	if (info.bmBitsPixel == 32)
		//	{
		//		mbAlpha = true;
		//	}
		//	else if (info.bmBitsPixel == 24)
		//	{
		//		mbAlpha = false;
		//	}
	  //
		//	HDC mainDC = application.GetHdc();
		//	mHdc = CreateCompatibleDC(mainDC);
	  //
		//	HBITMAP oldBitmap = (HBITMAP)SelectObject(mHdc, mBitmap);
		//	DeleteObject(oldBitmap);
	  //
		//}
	  //
	

		return S_OK;
	}

	void Texture::Bind(eShaderStage stage, UINT startSlot)
	{
		graphics::GetDevice()->SetShaderResource(stage, startSlot, mSRV.GetAddressOf());
	}

  //
	//COLORREF Texture::GetPixel(int x, int y)
	//{
	//	return ::GetPixel(mHdc, x, y);
	//}
  //
}