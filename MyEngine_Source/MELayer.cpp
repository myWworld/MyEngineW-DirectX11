#include "MELayer.h"

namespace ME
{
	ME::Layer::Layer()
	{
	}

	ME::Layer::~Layer()
	{

		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr)
				continue;

			delete gameObj;
			gameObj = nullptr;
		}
	}

	void ME::Layer::Initialize()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr)
				continue;

			gameObj->Initialize();
		}
	}

	void ME::Layer::Update()
		{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr)
				continue;

			GameObject::eState state = gameObj->GetState();

			if (state == GameObject::eState::Dead
					|| state == GameObject::eState::Paused)
			{
				continue;
			}

			gameObj->Update();
		}

	}

	void ME::Layer::LateUpdate()
	{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr)
				continue;


			GameObject::eState state = gameObj->GetState();

			if (state == GameObject::eState::Dead
				|| state == GameObject::eState::Paused)
			{
				continue;
			}

			gameObj->LateUpdate();
		}

	
	}

	void ME::Layer::Render(HDC hdc)
	{
		for (GameObject* gameObj : mGameObjects)
		{
			if (gameObj == nullptr)
				continue;


			GameObject::eState state = gameObj->GetState();
			if (state == GameObject::eState::Dead
				|| state == GameObject::eState::Paused
				|| state == GameObject::eState::NoRender)
			{
				continue;
			}

			gameObj->Render(hdc);
		}

	}
	
	void Layer::Destroy()
	{
		for (GameObjectIter iter = mGameObjects.begin()
			; iter != mGameObjects.end()
			; )
		{
			GameObject::eState active = (*iter)->GetState();

			if (active == GameObject::eState::Dead)
			{
				GameObject* deathObj = (*iter);
				iter = mGameObjects.erase(iter);

				delete deathObj;
				deathObj = nullptr;

				continue;
			}
			
	
			iter++;
			
		}
			
	}

	void Layer::EraseGameObject(GameObject* eraseGameObj)
	{
		std::erase_if(mGameObjects,
			[=](GameObject* gameObj)
			{
				return gameObj == eraseGameObj;
			});
	}

	void ME::Layer::AddGameObject(GameObject* gameObject)
	{

		if (gameObject == nullptr)
			return;

		mGameObjects.push_back(gameObject);

		
	}


	void Layer::findDeadGameObjects(OUT std::vector<GameObject*>& gameObjs)
	{
		for (GameObject* gameObj : mGameObjects)
		{
			GameObject::eState active = gameObj->GetState();

			if (active == GameObject::eState::Dead)
			{
				gameObjs.push_back(gameObj);
			}
		}
	}

	void Layer::deleteGameObjects(std::vector<GameObject*> deleteObjs)
	{
		for (GameObject* obj : deleteObjs)
		{
			delete obj;
			obj = nullptr;
		}
	}

	void Layer::eraseDeadGameObject()
	{
		std::erase_if(mGameObjects,
			[](GameObject* gameObj)
			{
				return (gameObj->IsDead());
			}
		);
	}

}