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
			End,

		};

		EnemyScript();
		~EnemyScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;


		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override ;
		void OnCollisionExit(Collider* other) override ;

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

		void directionChange(Vector3 newDir);

		void randomState();

	private:

		Vector2 mPrevMousePos;
		Vector2 mCurMousePos;
		float mMouseSpeed;

		class Animator3D* mAnimator;

		bool mbIsMoving;

		bool mbUseHands = true;
		bool mbHoldingGun = true;

		State mState = State::Idle;
		Direction mDirection = Direction::Forward;
		Direction mTargetDirection = Direction::Forward;

		Bone* mLeftHandBone;
		Bone* mRightHandBone;

		float translateTimer = 0.0f;
		float translateTime = 2.0f;

		bool mbTurn = false;
	};

}
