#pragma once

#include "../MyEngine_Source/MEScenes.h"


namespace ME
{
	class LoadingScene : public Scene
	{
	public:
		LoadingScene();
		~LoadingScene();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC mHdc)override;
		void Destroy()override;

		void OnEnter()override;
		void OnExit()override;

	private:

		void resourcesLoad(std::mutex& m);

	private:

		bool mbLoadCompleted;
		std::thread* mResourcesLoad;
		std::mutex mMutex;


	};
}
