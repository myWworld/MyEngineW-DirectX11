#include "MESpriteRenderer.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "CommonInclude.h"
#include "METexture.h"
#include "MERenderer.h"
#include "MECamera.h"


namespace ME {

	ME::SpriteRenderer::SpriteRenderer() 
		:mSize(Vector2::One)
		,mTexture(nullptr)
		,Component(enums::eComponentType::SpriteRenderer)
	{
	}

	ME::SpriteRenderer::~SpriteRenderer()
	{
	}

	void ME::SpriteRenderer::Initialize()
	{
	}

	void ME::SpriteRenderer::Update()
	{
	}

	void ME::SpriteRenderer::LateUpdate()
	{
	}

	void ME::SpriteRenderer::Render()
	{
		if (mTexture == nullptr)
			assert(false);


		//Transform* tr = GetOwner()->GetComponent<Transform>();
		//Vector2 pos = tr->GetPosition();
		//Vector2 scale = tr->GetScale();
		//float rot = tr->GetRotation();
		//
		//if(renderer::mainCamera)
		//	pos = renderer::mainCamera->CalculatePosition(pos);
		//
		//if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Png)
		//{
		//	Gdiplus::ImageAttributes imgAtt = {};
		//
		//	imgAtt.SetColorKey(Gdiplus::Color(230, 230, 230), Gdiplus::Color(255, 255, 255));
		//
		//	Gdiplus::Graphics graphics(hdc);
		//
		//	graphics.TranslateTransform(pos.x, pos.y);
		//	graphics.RotateTransform(rot);
		//	graphics.TranslateTransform(-pos.x, -pos.y);
		//
		//	graphics.DrawImage(mTexture->GetImage()
		//		, Gdiplus::Rect
		//		(
		//			pos.x
		//			, pos.y
		//			, mTexture->GetWidth() * scale.x
		//			, mTexture->GetHeight() * scale.y
		//		)
		//		,0, 0
		//		,mTexture->GetWidth()
		//		,mTexture->GetHeight()
		//		, Gdiplus::UnitPixel
		//		, nullptr
		//	);
		//}
		//else if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Bmp)
		//{
		//	HDC imgHdc = mTexture->GedHdc();
		//
		//	if (mTexture->IsAlpha())
		//	{
		//		//반투명한 효과를 원할때
		//		BLENDFUNCTION func = {};
		//		func.BlendOp = AC_SRC_OVER;
		//		func.BlendFlags = 0;
		//		func.AlphaFormat = 0;
		//		func.SourceConstantAlpha = 255;
		//
		//
		//		AlphaBlend(hdc
		//			, pos.x
		//			, pos.y
		//			, mTexture->GetWidth() * scale.x
		//			, mTexture->GetHeight() * scale.y
		//			, imgHdc
		//			, 0
		//			, 0
		//			,  mTexture->GetWidth() 
		//			,  mTexture->GetHeight() 
		//			, func);
		//	}
		//	else
		//	{
		//		TransparentBlt(hdc
		//			, pos.x
		//			, pos.y
		//			, mTexture->GetWidth() * scale.x
		//			, mTexture->GetHeight() * scale.y
		//			, mTexture->GedHdc()
		//			, 0, 0
		//			, mTexture->GetWidth() * mSize.x
		//			, mTexture->GetHeight() * mSize.y
		//			, RGB(255, 0, 255));
		//	}
		//}
		
	
	}

	
}