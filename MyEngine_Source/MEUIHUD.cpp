#include "MEUIHUD.h"
#include "MEResources.h"


namespace ME
{
	using namespace Gdiplus;

	ME::UIHUD::UIHUD()
		:UIBase(enums::eUIType::HpBar)
	{
	}

	ME::UIHUD::~UIHUD()
	{
	}

	void ME::UIHUD::OnInit()
	{
		SetPos(Vector2(10, 10));
		SetSize(Vector2(3.f, 3.0f));

		
		mTexture = Resources::Find<graphics::Texture>(L"HPBAR");
	}

	void ME::UIHUD::OnActive()
	{
	}

	void ME::UIHUD::OnInActive()
	{
	}

	void ME::UIHUD::OnUpdate()
	{
	}

	void ME::UIHUD::OnLateUpdate()
	{
	}

	void ME::UIHUD::OnRender()
	{

	

		
	}

	void ME::UIHUD::OnClear()
	{
	}

}