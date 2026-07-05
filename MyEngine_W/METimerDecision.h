#pragma once
#include "../MyEngine_Source/MEFSMDecision.h"


namespace ME
{
	class Transform;
	class TimerDecision :public FSMDecision
	{

	public:
		TimerDecision()
			: mCheckTime(2.0f)
			,mTimer(0.0f)
		{

			BindProperty("CheckTime", &mCheckTime);
		}


	protected:

		bool CheckDecision(FSMBrain* brain, GameObject* owner) override;


	public:

		float mCheckTime;

	private:

		float mTimer;

	};
}

