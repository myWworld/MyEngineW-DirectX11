#pragma once
#include "../MyEngine_Source/MEScenes.h"
#include "../MyEngine_Source/MECamera.h"

namespace ME
{
	class Model;
	class TitleScene:public Scene
	{
	public:
		TitleScene();
		~TitleScene();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render() override;


		void OnEnter() override;
		void OnExit() override;

	private:

		void MakeCharacter(Model* model);
		void MakeGun();

	private:

		class Camera* mCameraComp;
		class GameObject* mPlayer;
	
	};
}


