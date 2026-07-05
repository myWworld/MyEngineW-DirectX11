#pragma once
#include "../MyEngine_Source/MEFSMTask.h"
namespace ME
{
	class Transform;
	class MoveToTargetTask :public FSMTask
	{
		public:
		
			MoveToTargetTask ()
				: mSpeed(100.0f)
				, mStoppingDistance(50.0f)
				, target(nullptr)
				, mStoppingDistanceSqrt(0.0f)
				, ownerTr(nullptr)
			{
				bIsLoop = true;
				BindProperty("Speed", &mSpeed);
				BindProperty("StoppingDistance", &mStoppingDistance);
			}

			void OnEnter(FSMBrain* brain, GameObject* owner)override;
			void OnExecute(FSMBrain* brain, GameObject* owner) override;
			void OnExit(FSMBrain* brain, GameObject* owner)override {}


	public:

		float mSpeed;
		float mStoppingDistance;

	private:

		float mStoppingDistanceSqrt;
		Transform* target;
		Transform* ownerTr;
	};

}