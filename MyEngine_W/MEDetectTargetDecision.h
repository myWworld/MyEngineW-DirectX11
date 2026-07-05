#pragma once
#include "../MyEngine_Source/MEFSMDecision.h"


namespace ME
{
	class Transform;
	class DetectTargetDecision :public FSMDecision
	{

	public:
		DetectTargetDecision() : 
			mDetectRadius(50.0f)
			,target(nullptr)
			,ownerTr(nullptr)

		{
			BindProperty("DetectRadius", &mDetectRadius);
		}


	protected:

		bool CheckDecision(FSMBrain* brain, GameObject* owner) override;


	public:

		float mDetectRadius;


	private:

		Transform* target;
		Transform* ownerTr;

	};
}

