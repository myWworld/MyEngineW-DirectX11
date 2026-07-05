#pragma once
#include "../MyEngine_Source/MEFSMTask.h"
#include <string>

namespace ME
{
	class Animator;
	class FSMBrain;
	class MeleeAttackTask :public FSMTask
	{
	public:

		MeleeAttackTask()
			: mAnimName()
			, mbIsLoop(false)
			, mAnimator(nullptr)
		{
			BIND_PROP(mAnimName);
			BIND_PROP(mbIsLoop);
		}

		void OnEnter(FSMBrain* brain, GameObject* owner)override {}
		void OnExecute(FSMBrain* brain, GameObject* owner) override;
		void OnExit(FSMBrain* brain, GameObject* owner)override {}


	public:

		std::string mAnimName;
		float mbIsLoop;

	private:

		Animator* mAnimator;
	};

}