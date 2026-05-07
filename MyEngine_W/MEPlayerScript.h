#pragma once
#include "MEActorScript.h"
#include "MEMath.h"
#include "MECamera.h"
#include "MEBone.h"
#include "MEInputHandler.h"

namespace ME
{

	class Transform;
	class InputHandler;
	class WeaponScript;

	class PlayerScript :public ActorScript
	{
	public:
		


		enum class PlayerType
		{
			Player,
			Enemy,
			End,
		};

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
			ForwardLeft,
			ForwardRight,
			Left,
			Right,
			Back,
			BackLeft,
			BackRight,
			Up,

		};

		PlayerScript();
		~PlayerScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;
		void OnCollisionExit(Collider* other) override;
		
		void OnPrimaryAction() override;
		void OnToggleWeapon() override;

		bool IsMoving() { return mbIsMoving; }

		Bone* GetLeftHandBone() { return mLeftHandBone; }
		Bone* GetRightHandBone() { return mRightHandBone; }

		Bone* GetWeaponSocketBone() override;
		Vector3 GetAimDirection() override;

		void SetWeaponEquipment(WeaponScript* weapon)
		{
			mEquippedWeapon = weapon;
		}



	private:

		void Idle();
		void Move();
		void Attack();
		void Die();

		void Translate(math::Vector2 moveAxis);
		void randomAction();

		void directionChange();

	private:

		Vector2 mPrevMousePos;
		Vector2 mCurMousePos;
		float mMouseSpeed;

		class Animator3D* mAnimator;

		bool mbIsMoving;

		bool mbUseHands = true;

		State mState = State::Idle;
		Direction mDirection = Direction::Forward;
		Direction mTargetDirection = Direction::Forward;

		Transform* mPlayerTransform;
	

		Bone* mLeftHandBone;
		Bone* mRightHandBone;

		PlayerType mPlayerType;
		InputHandler mInputHandler;
	};

}
