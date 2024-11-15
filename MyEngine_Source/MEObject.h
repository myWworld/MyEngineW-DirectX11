#pragma once
#include "MEComponent.h"
#include "MESceneManager.h"
#include "MEScenes.h"
#include "CommonInclude.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MELayer.h"

namespace ME::object
{
	template <typename T>
	static T* Instantiate(ME::enums::eLayerType type)
	{
		T* gameObject = new T();
		gameObject->SetLayerType(type);
		Scene* activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(type);
		
		layer->AddGameObject(gameObject);

		return gameObject;
		
	}

	template <typename T>
	static T* Instantiate(ME::enums::eLayerType type, math::Vector2 position)
	{
		T* gameObject = new T();

		gameObject->SetLayerType(type);

		Scene* activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(type);

		layer->AddGameObject(gameObject);

		Transform* tr = gameObject->GetComponent<Transform>();
		tr->SetPosition(position);


		 
		return gameObject;

	}

	static void Destroy(GameObject* obj)
	{
		obj->SetDeath();
	}

	static void DontDestroyOnLoad(GameObject* gameObject)
	{
		Scene* activeScene = SceneManager::GetActiveScene();

		activeScene->EraseGameObject(gameObject);

		Scene* dontDestroyOnLoad = SceneManager::GetDontDestroyOnLoad();
		dontDestroyOnLoad->AddGameObject(gameObject, gameObject->GetLayerType());
	}


}