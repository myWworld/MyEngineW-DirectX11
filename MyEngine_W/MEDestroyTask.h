#pragma once
#include "../MyEngine_Source/MEFSMTask.h"

namespace ME
{
	class DestroyTask: public FSMTask
	{
	public:

		DestroyTask() {}
		~DestroyTask() {}

	protected:

		void OnEnter(FSMBrain* brain, GameObject* owner)override;
		 void OnExecute(FSMBrain* brain, GameObject* owner) override;
		 void OnExit(FSMBrain* brain, GameObject* owner)override {}
	};
}
