#pragma once
#include "../MyEngine_Source/MEFSMTask.h"
#include <string>
#include <vector>
#include <cstdlib>

namespace ME
{
	class Animator3D;
	class FSMBrain;
	class MeleeAttackTask :public FSMTask
	{
	public:

		MeleeAttackTask()
			: mAnimNames()
			, mbIsLoop(false)
			, mAnimator(nullptr)
		{
			BIND_PROP(mAnimNames);
			BIND_PROP(mbIsLoop);
		}

		void OnEnter(FSMBrain* brain, GameObject* owner)override;
		void OnExecute(FSMBrain* brain, GameObject* owner) override;
		void OnExit(FSMBrain* brain, GameObject* owner)override {}


	public:
		std::vector<std::string> mAnimNames; 
		bool mbIsLoop; 

	private:
		Animator3D* mAnimator;
		std::wstring mAnimNameW;
	};

}