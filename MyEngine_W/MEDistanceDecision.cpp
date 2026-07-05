#include "MEDistanceDecision.h"
#include "../MyEngine_Source/MEFSMBrain.h"
#include "../MyEngine_Source/StringUtility.h"
#include "../MyEngine_Source/MEBlackBoard.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/METransform.h"


namespace ME
{
    bool DistanceDecision::CheckDecision(FSMBrain* brain, GameObject* owner)
    {
        if (ownerTr == nullptr)
            ownerTr = owner->GetComponent<Transform>();

        if (target == nullptr)
            target = brain->GetBlackboard()->GetValue<Transform*>(StringUtility::HashStr(L"Target"));

        if (target == nullptr)
            return false;

        auto targetPos = target->GetPosition();
        auto ownerPos = ownerTr->GetPosition();

        float distSqrt = math::Vector3::DistanceSquared(targetPos, ownerPos);

        if (distSqrt <= mTargetDist * mTargetDist)
        {
            return true;
        }

        return false;
    }
}