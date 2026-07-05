#include "METimerDecision.h"
#include "../MyEngine_Source/METime.h"

namespace ME
{
	bool TimerDecision::CheckDecision(FSMBrain* brain, GameObject* owner)
	{
		mTimer += Time::DeltaTime();

		if (mTimer >= mCheckTime)
		{
			mTimer = 0.0f;

			return true;
		}

		return false;
	}
}