#include "MEPlayAnimTask.h"
#include "../MyEngine_Source/MEGameObject.h"
#include "../MyEngine_Source/MEAnimator3D.h"
#include "../MyEngine_Source/MEFSMBrain.h"
namespace ME
{

	void PlayAnimTask::OnEnter(FSMBrain* brain, GameObject* owner)
	{
		if (owner == nullptr) return;

		if (mAnimator == nullptr)
		{
			mAnimator = owner->GetComponent<Animator3D>();
		}

		if (mAnimNameW.empty() && !mAnimName.empty())
		{
			mAnimNameW.assign(mAnimName.begin(), mAnimName.end());
		}
	}

	void PlayAnimTask::OnExecute(FSMBrain* brain,GameObject* owner) 
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