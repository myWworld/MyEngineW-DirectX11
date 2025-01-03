#include "MESceneManager.h"
#include "MEDontDestroyOnLoad.h"

namespace ME
{

	std::map<std::wstring, Scene*> SceneManager::mScene = {};
	Scene* SceneManager::mActiveScene = nullptr;
	Scene* SceneManager::mDontDestroyOnLoad = nullptr;


	bool SceneManager::SetActiveScene(const std::wstring& name)
	{
		std::map<std::wstring, Scene*>::iterator iter = mScene.find(name);

		if (iter == mScene.end())
			return false;

		mActiveScene = iter->second;
		return true;
	}

	void SceneManager::Initialize()
	{
		mDontDestroyOnLoad = CreateScene<Scene>(L"DontDestroyOnLoad");
	}

	Scene* SceneManager::LoadScene(const std::wstring& name)
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		if (!(SetActiveScene(name)))
		{
			return nullptr;
		}


		mActiveScene->OnEnter();

		return mActiveScene;
	}


	std::vector<GameObject*> SceneManager::GetGameObject(enums::eLayerType layer)
	{
		
		std::vector<GameObject*>  gameObjects = mActiveScene->GetLayer(layer)->GetGameObject();

		std::vector<GameObject*>  dontDestroyOnLoad = mActiveScene->GetLayer(layer)->GetGameObject();

		gameObjects.insert(gameObjects.end(),dontDestroyOnLoad.begin(), dontDestroyOnLoad.end());

		return gameObjects;

	}

	 
	void SceneManager::Update()
	{
		mActiveScene->Update();
		mDontDestroyOnLoad->Update();
	}
	void SceneManager::LateUpdate()
	{
		mActiveScene->LateUpdate();
		mDontDestroyOnLoad->LateUpdate();
	}
	void SceneManager::Render()
	{
		mActiveScene->Render();
		mDontDestroyOnLoad->Render();
	}

	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
		mDontDestroyOnLoad->Destroy();
	}

	void SceneManager::Release()
	{
		for (auto& iter : mScene)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}
}
