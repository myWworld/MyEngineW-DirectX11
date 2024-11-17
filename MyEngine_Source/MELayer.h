#pragma once

#include "MEEntity.h"
#include "CommonInclude.h"
#include "MEGameObject.h"


namespace ME
{
	class Layer:public Entity
	{
	public:
		Layer();
		~Layer();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render();
		virtual void Destroy();

		void AddGameObject(GameObject* gameObject);
		void EraseGameObject(GameObject* eraseGameObj);

		const std::vector<GameObject*> GetGameObject() { return mGameObjects;  }

	private:

		void findDeadGameObjects(OUT std::vector<GameObject*>& gameObjs);
		void deleteGameObjects(std::vector<GameObject*> deleteObjs);
		void eraseDeadGameObject();

	
	private:

		std::vector<GameObject*> mGameObjects;
		

	};
	
		typedef std::vector<GameObject*>::iterator GameObjectIter;

}

