#pragma once
#include "MEUIBase.h"
#include "METexture.h"
#include "CommonInclude.h"
#include "MEGameObject.h"

namespace ME
{
	class UIHUD :public UIBase
	{
	public:
		UIHUD();
		~UIHUD();


		void OnInit() override;
		void OnActive()override;
		void OnInActive()override;
		void OnUpdate()override;
		void OnLateUpdate()override;
		void OnRender(HDC hdc)override;
		void OnClear()override;

		void SetOwner(GameObject* owner) { mOwner = owner; }


	private:
		graphics::Texture* mTexture;
		
		GameObject* mOwner;

	};

}

