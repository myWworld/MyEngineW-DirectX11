#pragma once
#include "../MyEngine_Source/MEScenes.h"
#include "../MyEngine_Source/MECamera.h"

namespace ME
{
	class TitleScene:public Scene
	{
	public:
		TitleScene();
		~TitleScene();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC mHdc) override;


		void OnEnter() override;
		void OnExit() override;

		static GameObject* GetRedTriangle() { return redTriangle; }

	private:

		class Camera* mCameraComp;
		class GameObject* titleBg;
		static GameObject* redTriangle;
	};
}

