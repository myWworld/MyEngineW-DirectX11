#pragma once
#include "../MyEngine_Source/MEFSMDecision.h"


namespace ME
{
	class Transform;
	class DistanceDecision :public FSMDecision
	{

	public:
		DistanceDecision() :
			mTargetDist(50.0f)
			, target(nullptr)
			, ownerTr(nullptr)

		{
			BindProperty("TargetDist", &mTargetDist);
		}


	protected:

		bool CheckDecision(FSMBrain* brain, GameObject* owner) override;


	public:

		float mTargetDist;


	private:

		Transform* target;
		Transform* ownerTr;

	};
}

