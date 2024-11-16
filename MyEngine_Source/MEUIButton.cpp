#include "MEUIButton.h"
#include "MEInput.h"

#include "MESceneManager.h"
#include "MEAnimator.h"
#include "MEResources.h"

#include "MESpriteRenderer.h"

#include "../MyEngine_W/METitleScene.h"

namespace ME
{
	UIButton::UIButton()
		:UIBase(enums::eUIType::Button)
	{
	}
	UIButton::~UIButton()
	{
	}
	void UIButton::OnInit()
	{
		SetPos(Vector2(255, 120));
		SetSize(Vector2(70,20));

		mTexture = Resources::Find<graphics::Texture>(L"STARTBUTTON");

		mOnClick = std::bind(&UIButton::ButtonClick, this);

		

	}
	void UIButton::OnActive()
	{
	}
	void UIButton::OnInActive()
	{
	}
	void UIButton::OnUpdate()
	{
		Vector2 mousePos = Input::GetMousePos();




		if (mousePos.x >= mPosition.x && mousePos.x <= mPosition.x + mSize.x
			&& mousePos.y >= mPosition.y && mousePos.y <= mPosition.y + mSize.y)
		{
		
			mbMouseOn = true;
		}
		else
		{
			mbMouseOn = false;
		}

		if (Input::GetKeyDown(eKeyCode::LeftMous))
		{
			if(mbMouseOn)
				mOnClick();
		}
	}
	void UIButton::OnLateUpdate()
	{
	}
	void UIButton::OnRender(HDC hdc)
	{
		
		if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Png)
		{
			Gdiplus::ImageAttributes imgAtt = {};

			imgAtt.SetColorKey(Gdiplus::Color(230, 230, 230), Gdiplus::Color(255, 255, 255));

			Gdiplus::Graphics graphics(hdc);

			if (mTexture->GetImage() == nullptr)
				return;

			graphics.DrawImage(mTexture->GetImage()
				, Gdiplus::Rect
				(
					mPosition.x
					, mPosition.y
					,  mSize.x
					, mSize.y
				)
				, 0, 0
				, mTexture->GetWidth()
				, mTexture->GetHeight()
				, Gdiplus::UnitPixel
				, nullptr
			);
		}
		else if (mTexture->GetTextureType() == graphics::Texture::eTextureType::Bmp)
		{
			TransparentBlt(hdc
				, mPosition.x, mPosition.y
				, mPosition.x + mSize.x
				, mPosition.y + mSize.y
				, mTexture->GedHdc()
				, 0, 0
				, mTexture->GetWidth()
				, mTexture->GetHeight()
				, RGB(255, 0, 255));
		}
	}
	void UIButton::OnClear()
	{
	}
	void UIButton::ButtonClick()
	{
		if (SceneManager::GetActiveScene()->GetName() == L"TitleScene")
		{
		
		}
		
	}
}