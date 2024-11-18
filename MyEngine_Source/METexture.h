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
		
		virtual HRESULT Save(const std::wstring& path) override;
		virtual HRESULT Load(const std::wstring& path) override;

		UINT GetWidth() const{ return mWidth; }
		void SetWidth(UINT width) { mWidth = width; }
		UINT GetHeight() const{ return mHeight; }
		void SetHeight(UINT height) { mHeight = height; }

		
		eTextureType GetTextureType() const{ return mType; }
		bool IsAlpha() const{ return mbAlpha; }

		//COLORREF GetPixel(int x, int y);

	private:

		eTextureType mType;

	
		UINT mWidth;
		UINT mHeight;

		bool mbAlpha;
	};
}

