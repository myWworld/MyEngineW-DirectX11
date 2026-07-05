#include "MERandomWanderTask.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/MEFSMBrain.h"
#include "../MyEngine_Source/METransform.h"
#include "../MyEngine_Source/METime.h"
#include "../MyEngine_Source/MEMath.h"
#include <cstdlib>

namespace ME
{

	void RandomWanderTask::OnEnter(FSMBrain* brain, GameObject* owner)
	{
		if (owner == nullptr) return;

		if (ownerTr == nullptr)
			ownerTr = owner->GetComponent<Transform>();

		if (!bHasOrigin)
		{
			mOriginPos = ownerTr->GetPosition();
			bHasOrigin = true;
		}

		float randX = ((rand() % 200) - 100) * 0.01f * mPatrolRadius;
		float randZ = ((rand() % 200) - 100) * 0.01f * mPatrolRadius;

		mTargetPos = mOriginPos + math::Vector3(randX, 0.0f, randZ);
	}

	void RandomWanderTask::OnExecute(FSMBrain* brain, GameObject* owner)
	{
		math::Vector3 ownerPos = ownerTr->GetPosition();

		if (math::Vector3::DistanceSquared(ownerPos, mTargetPos) < 50.0f * 50.0f)
		{
			bIsFinished = true;
			return;
		}


		math::Vector3 dir = mTargetPos - ownerPos;
		dir.y = 0.0f;

		if (dir.LengthSquared() < 0.0001f)
			return;

		dir.Normalize();

		float targetYawRad = atan2f(dir.x, dir.z);
		float targetYawDeg = XMConvertToDegrees(targetYawRad);

		targetYawDeg += 180.0f;

		math::Vector3 currentRot = ownerTr->GetRotation();
		float currentYawDeg = currentRot.y;

		float diff = targetYawDeg - currentYawDeg;
		while (diff > 180.0f) diff -= 360.0f;
		while (diff < -180.0f) diff += 360.0f;

		if (std::abs(diff) < 0.5f) //조그만 변화에도 지직거리듯 회전을 해버리는 문제가 생김을 방지
		{
			currentRot.y = targetYawDeg;
		}
		else
		{
			float turnSpeed = 20.0f; 
			currentRot.y += diff * turnSpeed * Time::DeltaTime();
		}

		ownerTr->SetRotation(currentRot);

		ownerPos += -ownerTr->Forward() * mSpeed * Time::DeltaTime();
		ownerTr->SetPosition(ownerPos);

	}
}