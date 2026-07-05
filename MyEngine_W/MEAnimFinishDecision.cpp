#include "MEAnimFinishDecision.h"

#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/MEFSMBrain.h"
#include "../MyEngine_Source/MEAnimator3D.h"

namespace ME
{
	bool AnimFinishDecision::CheckDecision(FSMBrain* brain, GameObject* owner)
	{
		if (mAnimator == nullptr)
			mAnimator = owner->GetComponent<Animator3D>();

		if (mAnimator->IsAnimationComplete()) return true;

		return false;
	}
}