#pragma once
#include "MEComponent.h"
#include "MEEntity.h"
#include "METexture.h"

namespace ME
{

	class Rigidbody : public Component
	{
	public:
		Rigidbody();
		~Rigidbody();

		void Initialize() override;
		void Update()  override;
		void LateUpdate()  override;
		void Render()  override;

		void SetMass(float mass) { mMass = mass; }
		void AddForce(Vector2 force) { mForce = force; }
		void SetGround(bool ground) { mbGround = ground; }
		void SetVelocity(Vector2 velocity) { mVelocity = velocity; }

		Vector2 GetVelocity() const{return mVelocity;}
		Vector2 GetForce() { return mForce; }
		bool IsGround() { return mbGround; }

		void StopMoving();

		void SetNeedGravity(bool isAffected, Vector2 gravity = Vector2(0,0))
		{
			if (isAffected == false)
				mbIsAffectedByGravity = false;
			
			if (isAffected == true)
				mbIsAffectedByGravity = true;
				
			mGravity = gravity;

		}

		
	private:
	
		bool mbGround;
		bool mbIsAffectedByGravity;

		float mMass;
		float mFriction;
		
		Vector2 mAccelation;
		Vector2 mForce;
		Vector2 mVelocity;
		Vector2 mLimitVelocity;
		Vector2 mGravity;

	};

}
