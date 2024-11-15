
#include "MEScenes.h"
#include "MECollisionManager.h"

namespace ME
{
	Scene::Scene()
		:mLayers{}
	{
		Scene::createLayers();
	}
	Scene::~Scene()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr)
				continue;

			delete layer;
			layer = nullptr;
		}
	}

	void Scene::Initialize()
	{
		for (Layer* layer : mLayers)
		{
			if (layer == nullptr)
				continue;

			layer->Initialize();
		}
	}

	void Scene::Update()
	{

		for (Layer* layer : mLayers)
		{
			if (layer == nullptr)
				continue;

			layer->Update();
		}
	}
	void Scene::LateUpdate()
	{

		for (Layer* layer : mLayers)
		{
			if (layer == nullptr)
				continue;

			layer->LateUpdate();
		}
	}
	void Scene::Render(HDC mHdc)
	{
		
			for (Layer* layer : mLayers)
			{
				if (layer == nullptr)
					continue;

				layer->Render(mHdc);
			}
	}
	void Scene::Destroy()
	{

		for (Layer* layer : mLayers)
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

	void Scene::AddGameObject(GameObject* gameObject, const enums::eLayerType type)
	{
		mLayers[(UINT)type]->AddGameObject(gameObject);
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
			mLayers[i] = new Layer();
		}
	}
}