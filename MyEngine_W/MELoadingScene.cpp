#include "MELoadingScene.h"
#include "MERenderer.h"
#include "MESceneManager.h"
#include "MEResources.h"
#include "METexture.h"

namespace ME
{
	LoadingScene::LoadingScene()
		:mbLoadCompleted(false)
		,mMutualExclusion()
		,mResourcesLoadThread()
	{
	}
	LoadingScene::~LoadingScene()
	{
		delete mResourcesLoadThread;
		mResourcesLoadThread = nullptr;

	}
	void LoadingScene::Initialize()
	{
		mResourcesLoadThread = new std::thread(&LoadingScene::resourcesLoad,this, std::ref(mMutualExclusion));
	}
	void LoadingScene::Update()
	{
		if (mbLoadCompleted)
		{
			mResourcesLoadThread->join();

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