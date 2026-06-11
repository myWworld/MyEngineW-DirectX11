#include "MELoadingScene.h"
#include "MERenderer.h"
#include "MESceneManager.h"
#include "MEResources.h"
#include "METexture.h"
#include "json.hpp"
#include "MEModel.h"
#include "METitleScene.h"
#include "../MyEngine_Source/MEAnimation3D.h"


#include "MEApplication.h"


extern ME::Application application;

using json = nlohmann::json;

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
		
		//	Resources::Load<graphics::Texture>(L"Character", L"..\\Resources\\characterBase.fbx");


			renderer::Initialize();

			std::ifstream file("..\\Resources\\ResourceList.json");

			if (!file.is_open()) {
				json datas = json::parse(file);
			}
			json datas = json::parse(file);

			for (auto& el : datas.items()) {
				std::cout << "Key: " << el.key() << std::endl;
			}

			for (const auto& texture : datas["Texture"])
			{
				std::wstring key = std::wstring(texture["key"].get<std::string>().begin(), texture["key"].get<std::string>().end());
				std::wstring path = std::wstring(texture["path"].get<std::string>().begin(), texture["path"].get<std::string>().end());
				Resources::Load<graphics::Texture>(key, path);
			}

			for (const auto& model : datas["Model"])
			{
				std::wstring key = std::wstring(model["key"].get<std::string>().begin(), model["key"].get<std::string>().end());
				std::wstring path = std::wstring(model["path"].get<std::string>().begin(), model["path"].get<std::string>().end());
				Resources::Load<Model>(key, path);
			}

			for (const auto& animation : datas["Animation"])
			{
				std::wstring key = std::wstring(animation["key"].get<std::string>().begin(), animation["key"].get<std::string>().end());
				std::wstring path = std::wstring(animation["path"].get<std::string>().begin(), animation["path"].get<std::string>().end());
				Resources::Load<Animation3D>(key, path);
			}

			//Resources::Load<graphics::Texture>(L"PISTOL", L"..\\Resources\\PistolTex.jpg");
			//Resources::Load<graphics::Texture>(L"GUN", L"..\\Resources\\M4Tex.png");
			//Resources::Load<graphics::Texture>(L"ALIEN", L"..\\Resources\\AlienTex.png");
			//Resources::Load<graphics::Texture>(L"SOLDIER", L"..\\Resources\\SoldierTex.jpg");
			//Resources::Load<graphics::Texture>(L"HPBAR", L"..\\Resources\\Hpbar.png");

			//Resources::Load<Model>(L"BulletModel", L"..\\Resources\\Bullet.fbx");
			//Resources::Load<Model>(L"AlienModel", L"..\\Resources\\alien.fbx");
			//Resources::Load<Model>(L"PistolModel", L"..\\Resources\\Pistol.fbx");
			//Resources::Load<Model>(L"SoldierModel", L"..\\Resources\\Soldier.fbx");

			//Resources::Load<Animation3D>(L"Idle", L"..\\Resources\\Animation\\Idle2.fbx");
			//Resources::Load<Animation3D>(L"FORWARDWALK", L"..\\Resources\\Animation\\ForwardWalk.fbx");
			//Resources::Load<Animation3D>(L"PISTOLWALK", L"..\\Resources\\Animation\\PistolWalk.fbx");
			//Resources::Load<Animation3D>(L"PISTOLIDLE", L"..\\Resources\\Animation\\PistolIdle.fbx");
			//Resources::Load<Animation3D>(L"PISTOLIDLE2", L"..\\Resources\\Animation\\PistolIdle2.fbx");
			//Resources::Load<Animation3D>(L"HIT", L"..\\Resources\\Animation\\HitReaction.fbx");
			//Resources::Load<Animation3D>(L"DANCE", L"..\\Resources\\Animation\\SillyDancing.fbx");

			
			SceneManager::CreateScene<TitleScene>(L"TitleScene");
		
		}
		m.unlock();

		SceneManager::SetActiveScene(L"LoadingScene");

		mbLoadCompleted = true;

	}
}