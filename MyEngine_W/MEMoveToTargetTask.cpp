#include "MEMoveToTargetTask.h"
#include "../MyEngine_Source/MEFSMBrain.h"
#include "../MyEngine_Source/StringUtility.h"
#include "../MyEngine_Source/MEBlackBoard.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/METransform.h"
#include "../MyEngine_Source/METime.h"

namespace ME
{

	void MoveToTargetTask::OnEnter(FSMBrain* brain, GameObject* owner)
	{
		if (ownerTr == nullptr)
			ownerTr = owner->GetComponent<Transform>();

		if (target == nullptr)
			target = brain->GetBlackboard()->GetValue<Transform*>(StringUtility::HashStr(L"Target"));

		mStoppingDistanceSqrt = mStoppingDistance * mStoppingDistance;
	}

	void MoveToTargetTask::OnExecute(FSMBrain* brain, GameObject* owner)
	{
		if (target == nullptr) return;

		auto targetPos = target->GetPosition();
		auto ownerPos = ownerTr->GetPosition();
		float distSqrt = math::Vector3::DistanceSquared(targetPos, ownerPos);



		if (distSqrt <= mStoppingDistanceSqrt)
		{
			bIsFinished = true;
			return;
		}

        Vector3 dir = (targetPos - ownerPos);
        dir.y = 0.0f;

        if (dir.LengthSquared() < 0.0001f)
            return;

        dir.Normalize();


        float targetYawRad = atan2f(dir.x, dir.z);
        float targetYawDeg = XMConvertToDegrees(targetYawRad);

        targetYawDeg += 180.0f;

        Vector3 currentRot = ownerTr->GetRotation();
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