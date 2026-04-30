
#include "MEScenes.h"
#include "MECollisionManager.h"
#include "MESceneManager.h"

namespace ME
{
	Scene::Scene()
		:mLayers{}
	{
		Scene::createLayers();
	}
	Scene::~Scene()
	{
	
	}

	void Scene::Initialize()
	{
		for (auto& layer : mLayers)
		{
			if (layer == nullptr) continue;
			layer->Initialize();
		}
	
	}

	void Scene::Update()
	{

		for (auto& layer : mLayers)
		{
			if (layer == nullptr)
				continue;

			layer->Update();
		}
	}
	void Scene::LateUpdate()
	{

		for (auto& layer : mLayers)
		{
			if (layer == nullptr)
				continue;

			layer->LateUpdate();
		}
	}
	void Scene::Render()
	{
		
			for (auto& layer : mLayers)
			{
				if (layer == nullptr)
					continue;

				layer->Render();
			}
	}
	void Scene::Destroy()
	{
		for (auto& layer : mLayers)
		{
			if (layer == nullptr)
				continue;

			layer->Destroy();
		}
	}

	void Scene::OnEnter()
	{

	 }
	void Scene::OnExit()
	{
		CollisionManager::Clear();
	}

	void Scene::AddGameObject(std::unique_ptr<GameObject> gameObject, const enums::eLayerType type)
	{
		if (gameObject == nullptr) return;

		mLayers[(UINT)type]->AddGameObject(std::move(gameObject));
	}

	void Scene::EraseGameObject(GameObject* gameObj)
	{
		enums::eLayerType layerType = gameObj->GetLayerType();
		mLayers[(UINT)layerType]->EraseGameObject(gameObj);
	}

	void Scene::createLayers()
	{
		mLayers.resize((UINT)enums::eLayerType::Max);

		for (size_t i = 0; i < (UINT)enums::eLayerType::Max; i++)
		{
			mLayers[i] = std::make_unique<Layer>();
		}
	}
}