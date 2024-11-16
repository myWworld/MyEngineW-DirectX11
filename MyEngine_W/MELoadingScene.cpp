#include "MELoadingScene.h"
#include "MERenderer.h"
#include "MESceneManager.h"
#include "MEResources.h"
#include "METexture.h"

namespace ME
{
	LoadingScene::LoadingScene()
		:mbLoadCompleted(false)
	{
	}
	LoadingScene::~LoadingScene()
	{
		delete mResourcesLoad;
		mResourcesLoad = nullptr;

	}
	void LoadingScene::Initialize()
	{
		mResourcesLoad = new std::thread(&LoadingScene::resourcesLoad,this, std::ref(mMutex));
	}
	void LoadingScene::Update()
	{
		if (mbLoadCompleted)
		{
			mResourcesLoad->join();

			SceneManager::LoadScene(L"TitleScene");
		}
	}
	void LoadingScene::LateUpdate()
	{
	}
	void LoadingScene::Render(HDC mHdc)
	{
	}
	void LoadingScene::Destroy()
	{
	}
	void LoadingScene::OnEnter()
	{
	}
	void LoadingScene::OnExit()
	{
	}
	void LoadingScene::resourcesLoad(std::mutex& m)
	{
		m.lock();
		{
			Resources::Load<graphics::Texture>(L"TITLE", L"..\\Resources\\Title.png");
			Resources::Load<graphics::Texture>(L"HPBAR", L"..\\Resources\\Hpbar.png");
			Resources::Load<graphics::Texture>(L"STARTBUTTON", L"..\\Resources\\StartButton.png");
		}
		m.unlock();

		mbLoadCompleted = true;

	}
}