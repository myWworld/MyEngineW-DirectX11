#pragma once
#include "../MyEngine_Source/CommonInclude.h"
#include "../MyEngine_Source/MESceneManager.h"

#include "METitleScene.h"

#include "METoolScene.h"
#include "MELoadingScene.h"
#include "MEPlayScene.h"


namespace ME
{
	void LoadScenes()
	{
		
		SceneManager::CreateScene<LoadingScene>(L"LoadingScene");

		SceneManager::LoadScene(L"LoadingScene");
	

	}
}
