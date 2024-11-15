#include "MEAnimation.h"
#include "METime.h"
#include "MEAnimator.h"
#include "METransform.h"
#include "MESpriteRenderer.h"
#include "MEGameObject.h"
#include "MERenderer.h"
#include "MECamera.h"
#include "MESceneManager.h"

namespace ME
{
	Animation::Animation()
		:Resource(enums::eResourceType::Animation)
		,mAnimator(nullptr)
		,mTexture(nullptr)
		,mAnimationSheet{}
		,mIndex(-1)
		,mTime(0.0f)
		,mbComplete(false)
	{
	}

	Animation::~Animation()
	{
	}
	HRESULT Animation::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	void Animation::Update()
	{

		if (mbComplete)
			return;

		mTime += Time::DeltaTime();

		if (mAnimationSheet[mIndex].duration < mTime)
		{
			mTime = 0;

			if (mIndex < mAnimationSheet.size()-1)
			{
				
				mIndex++;
			}
			else
			{
				mbComplete = true;
			}
		}
	}

	void Animation::Render(HDC hdc)
	{
			if (mTexture == nullptr)
				return;

		GameObject * gameObj = mAnimator->GetOwner();
		Transform* tr = gameObj->GetComponent<Transform>();
		

		Vector2 pos = tr->GetPosition();



		Vector2 scale = tr->GetScale();
		float rot = tr->GetRotation();

		Sprite sprite = mAnimationSheet[mIndex];

	
		if (renderer::mainCamera)
		{

			pos = renderer::mainCamera->CalculatePosition(pos);

		}

		graphics::Texture::eTextureType type = mTexture->GetTextureType();

		if (type == graphics::Texture::eTextureType::Bmp)
		{	

			HDC imgHdc = mTexture->GedHdc();

			if (mTexture->IsAlpha())
			{
				//반투명한 효과를 원할때
				BLENDFUNCTION func = {};		
				func.BlendOp = AC_SRC_OVER;
				func.BlendFlags = 0;
				func.AlphaFormat = 0;
				func.SourceConstantAlpha = 255;
				

				AlphaBlend(hdc
					, pos.x - (sprite.size.x / 2.0f) + sprite.offset.x
					, pos.y - (sprite.size.y / 2.0f) + sprite.offset.y
					, sprite.size.x * scale.x
					, sprite.size.y * scale.y
					, imgHdc
					, sprite.leftTop.x
					, sprite.leftTop.y
					, sprite.size.x
					, sprite.size.y
					, func);
			}
			else
			{
				TransparentBlt(hdc
					, pos.x - (sprite.size.x / 2.0f) + sprite.offset.x
					, pos.y - (sprite.size.y / 2.0f) + sprite.offset.y
					, sprite.size.x * scale.x
					, sprite.size.y * scale.y
					, imgHdc
					, sprite.leftTop.x
					, sprite.leftTop.y
					, sprite.size.x
					, sprite.size.y
					, RGB(255, 0, 255));
			}

		}
		else if (type == graphics::Texture::eTextureType::Png)
		{
			//내가 원하는 픽셀을 투명화 시킬때
			Gdiplus::ImageAttributes imgAtt = {};

			imgAtt.SetColorKey(Gdiplus::Color(100, 100, 100), Gdiplus::Color(255, 255, 255));

			Gdiplus::Graphics graphics(hdc);

			graphics.TranslateTransform(pos.x, pos.y);
			graphics.RotateTransform(rot);
			graphics.TranslateTransform(-pos.x, -pos.y);

			graphics.DrawImage(mTexture->GetImage()
				,Gdiplus::Rect
				(
					  pos.x - (sprite.size.x / 2.0f) + sprite.offset.x
					, pos.y - (sprite.size.y / 2.0f) + sprite.offset.y
					, sprite.size.x * scale.x
					, sprite.size.y * scale.y
				)
				,sprite.leftTop.x
				, sprite.leftTop.y
				, sprite.size.x
				, sprite.size.y
				,Gdiplus::UnitPixel
				,nullptr
			);
		}

		//Rectangle(hdc, pos.x, pos.y, pos.x +5 , pos.y+5);
	}

	void Animation::CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size
		, Vector2 offset,float duration, UINT spriteLength1, UINT spriteLength2)
	{
		mTexture = spriteSheet;
		
		std::wstring Direction = name.substr(name.size() - 1);

		if (Direction == L"R")
		{
			InsertRightAnimationSheets(leftTop, size, offset, duration, spriteLength1, spriteLength2);
		}
		else if (Direction == L"L")
		{
			InsertLeftAnimationSheets(leftTop, size, offset, duration, spriteLength1, spriteLength2);
		}
		
		
	}
	void Animation::Reset()
	{

		mTime = 0;
		mIndex = 0;
		mbComplete = false;
	}

	void Animation::InsertRightAnimationSheets(Vector2 leftTop, Vector2 size
		, Vector2 offset, float duration
		, UINT spriteLength1
		, UINT spriteLength2)
	{

		int count = (spriteLength2 > 0) ? 2 : 1;
		UINT Length = spriteLength1;
		Vector2 pos = leftTop;


		for (size_t j = 0; j < count; j++)
		{

			if (j == 1)
			{
				Length = spriteLength2;
				pos.y += size.y;

				if (pos.x > 0)
				{
					pos.x = 0;
				}

			}

			for (size_t i = 0; i < Length; i++)
			{
				Sprite sprite = {};
				sprite.leftTop.x = pos.x + (size.x * i);
				sprite.leftTop.y = pos.y;
				sprite.size = size;
				sprite.offset = offset;
				sprite.duration = duration;

				mAnimationSheet.push_back(sprite);
			}
		}
	}


	void Animation::InsertLeftAnimationSheets(Vector2 leftTop, Vector2 size
		, Vector2 offset, float duration
		, UINT spriteLength1
		, UINT spriteLength2)
	{
		int count = (spriteLength2 > 0) ? 2 : 1;
		UINT Length = spriteLength1;
		Vector2 pos = leftTop;
		
		UINT width = mTexture->GetWidth();

		for (size_t j = 0; j < count; j++)
		{

			if (j == 1)
			{
				Length = spriteLength2;
				pos.y += size.y;

				if (pos.x < width - size.x)
				{
					pos.x = width - size.x;
				}

			}

			for (size_t i = 0; i < Length; i++)
			{
				Sprite sprite = {};
				sprite.leftTop.x = pos.x - (size.x * i);
				sprite.leftTop.y = pos.y;
				sprite.size = size;
				sprite.offset = offset;
				sprite.duration = duration;

				mAnimationSheet.push_back(sprite);
			}
		}
	}


}