#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "MEMath.h"
#include "MECamera.h"
#include "MEBone.h"

namespace ME
{


	class EnemyScript :public Script
	{
	public:


		enum class State
		{
			Idle,
			Patrol,
			Run,
			Attack,

		};

		enum class Direction
		{
			Forward,
			ForwardLeft,
			ForwardRight,
			Left,
			Right,
			Back,
			BackLeft,
			BackRight,
			Up,

		};

		EnemyScript();
		~EnemyScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		bool IsMoving() { return mbIsMoving; }
		bool IsUsingGun() { return mbHoldingGun; }

		Bone* GetLeftHandBone() { return mLeftHandBone; }
		Bone* GetRightHandBone() { return mRightHandBone; }

	private:

		void Idle();
		void Move();
		void Attack();
		void Die();

		void Translate();

		void directionChange();

	private:

		Vector2 mPrevMousePos;
		Vector2 mCurMousePos;
		float mMouseSpeed;

		class Animator3D* mAnimator;

		bool mbIsMoving;

		bool mbUseHands = true;
		bool mbHoldingGun = false;

		State mState = State::Idle;
		Direction mDirection = Direction::Forward;
		Direction mTargetDirection = Direction::Forward;

		Bone* mLeftHandBone;
		Bone* mRightHandBone;
	};

}
