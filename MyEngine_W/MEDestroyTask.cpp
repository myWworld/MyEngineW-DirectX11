#include "MEDestroyTask.h"
#include "../MyEngine_Source/MEGameObject.h"

namespace ME
{
	void DestroyTask::OnEnter(FSMBrain* brain, GameObject* owner)
	{
		if (owner == nullptr) return;
		owner->SetActive(false);
		owner->SetDeath();
		bIsFinished = true;
	}
	void DestroyTask::OnExecute(FSMBrain* brain, GameObject* owner)
	{

	}
}