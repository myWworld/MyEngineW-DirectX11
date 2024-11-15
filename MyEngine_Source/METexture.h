#pragma once
#include "MEResource.h"

namespace ME::graphics
{

	class Texture: public Resource
	{
	public:
		enum class eTextureType
		{
			Bmp,
			Png,
			None,
		};

		static Texture* Create(const std::wstring& name, UINT width, UINT height);

		Texture();
		~Texture();
		
		virtual HRESULT Load(const std::wstring& path) override;

		UINT GetWidth() { return mWidth; }
		void SetWidth(UINT width) { mWidth = width; }
		UINT GetHeight() { return mHeight; }
		void SetHeight(UINT height) { mHeight = height; }

		HDC GedHdc() { return mHdc; }
		eTextureType GetTextureType() { return mType; }
		Gdiplus::Image* GetImage() { return mImage; }

		bool IsAlpha() { return mbAlpha; }

		COLORREF GetPixel(int x, int y);

	private:

		eTextureType mType;

		Gdiplus::Image* mImage;
		HBITMAP mBitmap;
		HDC mHdc;

		UINT mWidth;
		UINT mHeight;

		bool mbAlpha;
	};
}

