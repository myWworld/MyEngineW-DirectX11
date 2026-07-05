#include "MEDetectTargetDecision.h"
#include "../MyEngine_Source/MEFSMBrain.h"
#include "../MyEngine_Source/StringUtility.h"
#include "../MyEngine_Source/MEBlackBoard.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/METransform.h"
#include "../MyEngine_Source/MESceneManager.h"
#include "../MyEngine_Source/MEScenes.h"
#include "../MyEngine_Source/MELayer.h"


namespace ME
{

    bool DetectTargetDecision::CheckDecision(FSMBrain* brain, GameObject* owner)
    {
        if (ownerTr == nullptr)
            ownerTr = owner->GetComponent<Transform>();


        auto playerVec = SceneManager::GetActiveScene()->GetLayer(enums::eLayerType::Player)->GetGameObject();

        if (playerVec.empty())
            return false;

        GameObject* player = playerVec[0];

        if (player == nullptr)
            return false; 

        auto targetPos = player->GetComponent<Transform>()->GetPosition();
        auto ownerPos = ownerTr->GetPosition();

        float distSqrt = math::Vector3::DistanceSquared(targetPos, ownerPos);

        if (distSqrt <= mDetectRadius * mDetectRadius)
        {
            brain->GetBlackboard()->SetValue(StringUtility::HashStr(L"Target"), player->GetComponent<Transform>());
            return true;
        }

        return false;
    }
}