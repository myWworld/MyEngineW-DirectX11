#pragma once
#include "../MyEngine_Source/MEFSMTask.h"
#include <string>

namespace ME
{
	class Animator3D;
	class PlayAnimTask :public FSMTask
	{
	public:

		PlayAnimTask()
			: mAnimName()
			, mbIsLoop(false)
			,mAnimator(nullptr)
		{
			BindProperty("AnimName", &mAnimName);
			BindProperty("IsLoop", &mbIsLoop);
		}

		void OnEnter(FSMBrain* brain, GameObject* owner)override;
		void OnExecute(FSMBrain* brain, GameObject* owner) override;
		void OnExit(FSMBrain* brain, GameObject* owner)override {}


	public:

		std::string mAnimName;
		bool mbIsLoop;

	private:

		Animator3D* mAnimator;
		std::wstring mAnimNameW;
	};

}