#pragma once
#include "../MyEngine_Source/MEFSMDecision.h"


namespace ME
{
	class Transform;
	class Animator3D;
	class AnimFinishDecision :public FSMDecision
	{

	public:
		AnimFinishDecision() :
			mAnimator(nullptr)
			, ownerTr(nullptr)

		{
		
		}


	protected:

		bool CheckDecision(FSMBrain* brain, GameObject* owner) override;


	public:

	


	private:

		Animator3D* mAnimator;
		Transform* ownerTr;

	};
}

