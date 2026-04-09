#include "MECollisionManager.h"
#include "MEScenes.h"
#include "MESceneManager.h"
#include "MECollider.h"
#include "MEBoxCollider2D.h"
#include "MEBoxCollider3D.h"

namespace ME
{
	 std::bitset<(UINT)enums::eLayerType::Max> CollisionManager::mCollisionLayerMatrix[(UINT)enums::eLayerType::Max] = {0};
	 std::unordered_map<UINT64, bool> CollisionManager::mCollisionMap = {};
	 QuadTree* CollisionManager::mQuadTree = nullptr;

	 void ME::CollisionManager::Iniatialize()
	{
		 if (mQuadTree == nullptr)
		 {

			 math::Vector2 worldMin(-10000.0f, -10000.0f); // 맵 최소 좌표
			 math::Vector2 worldMax(10000.0f, 10000.0f);   // 맵 최대 좌표

			 mQuadTree = new QuadTree(0, worldMin, worldMax);
		 }
	}

	void ME::CollisionManager::Update()
	{
		

		for (UINT row = 0; row < (UINT)enums::eLayerType::Max; row++)
		{
			for (UINT col = 0; col < (UINT)enums::eLayerType::Max; col++)
			{
				if (mCollisionLayerMatrix[row][col] == true)
				{
					LayerCollision((enums::eLayerType)row, (enums::eLayerType)col);
				}
			}
		}
	}

	void ME::CollisionManager::LateUpdate()
	{
	}

	void ME::CollisionManager::Render()
	{
	}

	void CollisionManager::CollisionLayerCheck(enums::eLayerType left, enums::eLayerType right, bool enable)
	{
		int row = 0;
		int col = 0;

		if (left <= right)
		{
			row = (UINT)left;
			col = (UINT)right;
		}
		else
		{
			row = (UINT)right;
			col = (UINT)left;
		}

 		mCollisionLayerMatrix[row][col] = enable;
	}


	void CollisionManager::LayerCollision( enums::eLayerType left, enums::eLayerType right)
	{
		const std::vector<GameObject*>& leftObjs = SceneManager::GetGameObject(left);
		const std::vector<GameObject*>& rightObjs = SceneManager::GetGameObject(right);

		mQuadTree->Clear();

		for (GameObject* rightObj : rightObjs)
		{
			if (!rightObj->IsActive()) continue;

			Collider* rightCol = rightObj->GetComponent<Collider>();
			if (rightCol != nullptr)
			{
				mQuadTree->Insert(rightCol); 
			}

		}

		for (GameObject* leftObj : leftObjs)
		{
			if (!leftObj->IsActive()) continue;

			Collider* leftCol = leftObj->GetComponent<Collider>();
			if (leftCol == nullptr) continue;

			std::vector<Collider*> returnObjects;
			mQuadTree->Retrieve(returnObjects, leftCol);

			for (Collider* rightCol : returnObjects)
			{
				if (rightCol == nullptr || leftObj == rightCol->GetOwner()) continue;

				ColliderCollision(leftCol, rightCol);
				
			}
		}
	
	}

	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
	{
		//두 충돌체 번호를 가져온 ID를 확인해서 CollisionId 값을 세팅
		CollisionID id = {  };

		id.left = left->GetID();
		id.right = right->GetID();

		//해당 id로 충돌체 정보를 검색해준다.
		//만약에 충돌체 정보가 없다면 충돌정보를 생성

		auto iter = mCollisionMap.find(id.id);

		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert({ id.id, false });
			iter = mCollisionMap.find(id.id);
		}

		//충돌체크를 해주면된다.

		if (Intersect(left, right))
		{
			if (iter->second == false)
			{
				left->OnCollisionEnter(right);
				right->OnCollisionEnter(left);

				iter->second = true;
			}
			else
			{
				left->OnCollisionStay(right);
				right->OnCollisionStay(left);
			}
		}
		else
		{
			if (iter->second == true)
			{
				left->OnCollisionExit(right);
				right->OnCollisionExit(left);

				iter->second = false;
			}
		}
	}

	bool CollisionManager::Intersect(Collider* left, Collider* right)
	{
		return left->Intersect(right);

	}

	void CollisionManager::Clear()
	{
		mCollisionMap.clear();
		mCollisionLayerMatrix->reset();
	}

}