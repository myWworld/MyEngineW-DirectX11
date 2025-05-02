#include "MELoadingScene.h"
#include "MERenderer.h"
#include "MESceneManager.h"
#include "MEResources.h"
#include "METexture.h"
#include "MESceneManager.h"
#include "METitleScene.h"



#include "MEApplication.h"


extern ME::Application application;

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

	}
	void LoadingScene::LateUpdate()
	{
	}
	void LoadingScene::Render()
	{
		if (mbLoadCompleted)
		{
			mResourcesLoadThread->join();

			SceneManager::LoadScene(L"TitleScene");
		}
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

		while (true)
		{
			if (application.IsLoaded() == true)
			{
				break;
			}
		}

		m.lock();
		{
			Resources::Load<graphics::Texture>(L"TITLE", L"..\\Resources\\Statge1_1.png");
		//	Resources::Load<graphics::Texture>(L"Character", L"..\\Resources\\characterBase.fbx");
			Resources::Load<graphics::Texture>(L"HUMAN", L"..\\Resources\\humanTex.jpg");
			Resources::Load<graphics::Texture>(L"ALIEN", L"..\\Resources\\AlienTex.png");
			Resources::Load<graphics::Texture>(L"MALE", L"..\\Resources\\man_tex.png");
			Resources::Load<graphics::Texture>(L"SOLDIER", L"..\\Resources\\SoldierTex.jpg");
			renderer::Initialize();

			
			SceneManager::CreateScene<TitleScene>(L"TitleScene");
		
		}
		m.unlock();

		SceneManager::SetActiveScene(L"LoadingScene");

		mbLoadCompleted = true;

	}
}