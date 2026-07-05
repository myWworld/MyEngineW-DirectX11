#pragma once
#include "../MyEngine_Source/MEFSMTask.h"
#include "../MyEngine_Source/MEMath.h"
#include <string>

namespace ME
{
	class Transform;
	class Vector3;
	class RandomWanderTask :public FSMTask
	{
	public:

		RandomWanderTask()
			: mSpeed(20.0f)
			, mPatrolRadius(40.0f)

		{
			bIsLoop = true;
			BindProperty("Speed", &mSpeed);
			BindProperty("PatrolRadius", &mPatrolRadius);
		}

		void OnEnter(FSMBrain* brain, GameObject* owner)override;
		void OnExecute(FSMBrain* brain, GameObject* owner) override;
		void OnExit(FSMBrain* brain, GameObject* owner)override {}


	public:

		float mSpeed;
		float mPatrolRadius;

	private:

		Transform* ownerTr;
		math::Vector3 mOriginPos; // 스폰된 최초 위치 (여기서 너무 멀어지지 않게)
		math::Vector3 mTargetPos; // 이번에 걸어갈 랜덤 목표 위치
		bool bHasOrigin;
	};

}