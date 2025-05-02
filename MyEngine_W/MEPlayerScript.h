#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "MEMath.h"

namespace ME
{


	class PlayerScript :public Script
	{
	public:


		enum class State
		{
			Idle,
			Walk,
			Run,
			Attack,
			
		};

		enum class Direction
		{
			Forward,
			Left,
			Right,
			Back,
			Up,

		};

		PlayerScript();
		~PlayerScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		


	private:

		void Idle();
		void Move();
		void Attack();
		void Die();

		void Translate();

	private:

		Vector2 mPrevMousePos;
		Vector2 mCurMousePos;
		float mMouseSpeed;

		class Animator3D* mAnimator;

		State mState = State::Idle;
		Direction mDirection = Direction::Forward;
	};

}
