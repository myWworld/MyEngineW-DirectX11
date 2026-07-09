#include "MEMeleeAttackTask.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/MEAnimator3D.h"
#include "../MyEngine_Source/MEFSMBrain.h"
namespace ME
{

	void MeleeAttackTask::OnEnter(FSMBrain* brain, GameObject* owner)
	{
		if (mAnimator == nullptr)
			mAnimator = owner->GetComponent<Animator3D>();

		if (mAnimNames.empty() || mAnimator == nullptr) return;

		int randomIndex = rand() % mAnimNames.size();

		mAnimNameW = std::wstring(mAnimNames[randomIndex].begin(), mAnimNames[randomIndex].end());

	}

	void MeleeAttackTask::OnExecute(FSMBrain* brain, GameObject* owner)
	{
		if (owner == nullptr) return;

		if (mAnimator == nullptr)
			mAnimator = owner->GetComponent<Animator3D>();

		auto activeAnim = mAnimator->GetActiveAnimation();
		auto targetAnim = mAnimator->FindAnimation(mAnimNameW);

		if (activeAnim != targetAnim)
		{
			mAnimator->PlayAnimation(mAnimNameW, mbIsLoop);

		}

	}
}