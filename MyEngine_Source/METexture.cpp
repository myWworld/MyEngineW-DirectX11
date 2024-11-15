#include "METexture.h"
#include "MEApplication.h"
#include "MEResources.h"

extern ME::Application application;

namespace ME::graphics
{
	Texture* Texture::Create(const std::wstring& name, UINT width, UINT height)
	{
		Texture* image = Resources::Find<Texture>(name);

		if (image)
			return image;

		image = new Texture();
		image->SetName(name);
		image->SetHeight(height);
		image->SetWidth(width);

		HDC hdc = application.GetHdc();
		HWND hwnd = application.GetHwnd();

		image->mBitmap = CreateCompatibleBitmap(hdc, width, height);
		image->mHdc = CreateCompatibleDC(hdc);

		HBRUSH transparentBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, transparentBrush);

		::Rectangle(image->mHdc, -1, -1, image->GetWidth() +1, image->GetHeight()+1);

		SelectObject(hdc, oldBrush);
	
		HBITMAP oldBitmap = (HBITMAP)SelectObject(image->mHdc, image->mBitmap);
		DeleteObject(oldBitmap);

		Resources::Insert(name, image);

		return image;

	}

	Texture::Texture()
		:Resource(enums::eResourceType::Texture)
		,mbAlpha(false)
	{

	}

	Texture::~Texture()
	{
	}

	HRESULT Texture::Load(const std::wstring& path)
	{
		//bmp 일때
		std::wstring ext 
			= path.substr(path.find_last_of(L".") + 1);
		

		//png 일때
			
		if (ext == L"png")
		{
			mType = eTextureType::Png;
			mImage = Gdiplus::Image::FromFile(path.c_str());

			if (mImage == nullptr) { return S_FALSE; }

			mWidth = mImage->GetWidth();
			mHeight = mImage->GetHeight();
		}
		else if (ext == L"bmp")
		{
			mType = eTextureType::Bmp;

			mBitmap = (HBITMAP)LoadImageW(nullptr, path.c_str(), IMAGE_BITMAP,
				0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

			if (mBitmap == nullptr) { return S_FALSE; }

			BITMAP info = {};
			GetObject(mBitmap, sizeof(BITMAP), &info);

			mWidth = info.bmWidth;
			mHeight = info.bmHeight;

			if (info.bmBitsPixel == 32)
			{
				mbAlpha = true;
			}
			else if (info.bmBitsPixel == 24)
			{
				mbAlpha = false;
			}

			HDC mainDC = application.GetHdc();
			mHdc = CreateCompatibleDC(mainDC);

			HBITMAP oldBitmap = (HBITMAP)SelectObject(mHdc, mBitmap);
			DeleteObject(oldBitmap);

		}

	

		return S_OK;
	}

	COLORREF Texture::GetPixel(int x, int y)
	{
		return ::GetPixel(mHdc, x, y);
	}

}