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
	 
	 void ME::CollisionManager::Iniatialize()
	{
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

		for (GameObject* leftObj : leftObjs)
		{
			if (leftObj->IsActive() == false)
			{
				continue;
			}

			Collider* leftCol = leftObj->GetComponent<Collider>();

			if (leftCol == nullptr)
			{
				continue;
			}



			for (GameObject* rightObj : rightObjs)
			{
				{
					if (rightObj->IsActive() == false)
					{
						continue;
					}

					Collider* rightCol = rightObj->GetComponent<Collider>();

					if (rightCol == nullptr)
					{
						continue;
					}


					if (leftObj == rightObj)
						continue;

					ColliderCollision(leftCol, rightCol);
				}
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

		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		Vector3 leftPos = leftTr->GetPosition() + left->GetOffset();
		Vector3 rightPos = rightTr->GetPosition() + right->GetOffset();

		Vector3 leftSize = left->GetSize(); //* 100.0f;
		Vector3 rightSize = right->GetSize(); //* 100.0f;

		BoxCollider2D* leftBC = (static_cast<BoxCollider2D*>(left));
		BoxCollider2D* rightBC = (static_cast<BoxCollider2D*>(right));

		BoxCollider3D* leftBC3D = (static_cast<BoxCollider3D*>(left));
		BoxCollider3D* rightBC3D = (static_cast<BoxCollider3D*>(right));

		enums::eColliderType leftType = left->GetColliderType();
		enums::eColliderType rightType = right->GetColliderType();


#pragma region Handle 2D collider
		if (leftType == enums::eColliderType::Rect2D
			&& rightType == enums::eColliderType::Rect2D)
		{
			Vector3 leftCenterPos = leftPos  + (leftSize / 2.0f);
			Vector3 rightCenterPos = rightPos + (rightSize / 2.0f);

	
				if (leftBC->IsRotate())
				{

					leftCenterPos = leftBC->GetCentralPoint();

					leftSize.y = leftBC->GetHeight();
					leftSize.x = leftBC->GetWidth();

				}

				if (rightBC->IsRotate())
				{
					rightCenterPos = rightBC->GetCentralPoint();

					rightSize.y = rightBC->GetHeight();
					rightSize.x = rightBC->GetWidth();

				}

			

				//AABB 충돌 rect-rect
				if (fabs(leftCenterPos.x - rightCenterPos.x) < fabs(leftSize.x / 2.0f + rightSize.x / 2.0f)
					&& fabs(leftCenterPos.y - rightCenterPos.y) < fabs(leftSize.y / 2.0f + rightSize.y / 2.0f))
				{
					return true;
				}
			




		}

		if (leftType == enums::eColliderType::Circle2D
			&& rightType == enums::eColliderType::Circle2D)
		{
			//circle - circle
			Vector3 leftCirclePos = leftPos + (leftSize / 2.0f);
			Vector3 rightCirclePos = rightPos + (rightSize / 2.0f);

			float distance = (leftCirclePos - rightCirclePos).Length();

			if (distance <= (leftSize.x / 2.0f + rightSize.x / 2.0f))
			{
				return true;
			}

		}

		if (leftType == enums::eColliderType::Circle2D && rightType == enums::eColliderType::Rect2D)
		{
			//circle - rect
			Vector3 leftCirclePos = leftPos + (leftSize / 2.0f);

			float r = leftSize.x / 2.0f;


			if ( (leftCirclePos.x >= rightPos.x - r && leftCirclePos.x <= rightPos.x + r) 
				&& (leftCirclePos.y >= rightPos.y - r && leftCirclePos.y <= rightPos.y + r))
			{
				return true;
			}
			
		}


		if (leftType == enums::eColliderType::Rect2D && rightType == enums::eColliderType::Circle2D)
		{
			//circle - rect
			Vector3 rightCirclePos = rightPos + (rightSize / 2.0f);

			float r = rightSize.x / 2.0f;


			if ((rightCirclePos.x >= leftPos.x - r && rightCirclePos.x <= leftPos.x + r)
				&& (rightCirclePos.y >= leftPos.y - r && rightCirclePos.y <= leftPos.y + r))
			{
				return true;
			}

		}
#pragma endregion

#pragma region Handle 3D collider

		if (leftType == enums::eColliderType::Box3D
			&& rightType == enums::eColliderType::Box3D)
		{
			Vector3 LHeight = Vector3(0, leftSize.y, 0);
			Vector3 RHeight = Vector3(0, rightSize.y, 0);

			Vector3 leftCenterPos = leftPos + (LHeight / 2.0f);
			Vector3 rightCenterPos = rightPos + (RHeight / 2.0f);


			if (leftBC3D->IsRotate())
			{

				leftCenterPos = leftBC3D->GetCentralPoint();

				leftSize.y = leftBC3D->GetHeight();
				leftSize.x = leftBC3D->GetWidth();

			}

			if (rightBC3D->IsRotate())
			{
				rightCenterPos = rightBC3D->GetCentralPoint();

				rightSize.y = rightBC3D->GetHeight();
				rightSize.x = rightBC3D->GetWidth();

			}



			//AABB 충돌 rect-rect
			if (fabs(leftCenterPos.x - rightCenterPos.x) < fabs(leftSize.x / 2.0f + rightSize.x / 2.0f)
				&& fabs(leftCenterPos.y - rightCenterPos.y) < fabs(leftSize.y / 2.0f + rightSize.y / 2.0f) 
				&& fabs(leftCenterPos.z - rightCenterPos.z) < fabs(leftSize.z / 2.0f + rightSize.z / 2.0f))
			{
				return true;
			}

		}

#pragma endregion



		return false;
	}

	void CollisionManager::Clear()
	{
		mCollisionMap.clear();
		mCollisionLayerMatrix->reset();
	}

}