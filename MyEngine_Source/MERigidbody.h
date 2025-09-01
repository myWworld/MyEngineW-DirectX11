#pragma once
#include "MEComponent.h"
#include "MEEntity.h"
#include "METexture.h"

namespace ME
{

	class Rigidbody : public Component
	{
	public:

		enum class eForceMode
		{
			Force,      // F = ma �� �� ������ ������ ���� (dt �ݿ�)
			Impulse,    // ���� ���޽� �� �ӵ��� ��� �ݿ� (dt ����)
			VelocityChange, // ���� �����ϰ� �ٷ� �ӵ��� �ݿ�
			Acceleration,   // ���� ����, ���ӵ��� ����
		};

		Rigidbody();
		~Rigidbody();

		void Initialize() override;
		void Update()  override;
		void LateUpdate()  override;
		void Render()  override;

		void SetMass(float mass) {
			
			mMass = mass; 
			mGravity = Vector3(0.0f, gravity * mMass, 0.0f);
		}
		void AddForce(Vector3 force) { sumAccel += force; }

		void AddForce(const Vector3& v, eForceMode mode)
		{
			switch (mode) {
			case eForceMode::Force:          sumForce += v;      break; // N
			case eForceMode::Acceleration:   sumAccel += v;      break; // m/s^2
			case eForceMode::Impulse:        sumImpulse += v;      break; // N��s
			case eForceMode::VelocityChange: sumVelChange += v;    break; // m/s
			}
		}
		
		void SetGround(bool ground) { mbGround = ground; }
		void SetVelocity(Vector3 velocity) { mVelocity = velocity; }
		

		Vector3 GetVelocity() const{return mVelocity;}
		Vector3 GetForce() { return mForce; }
		bool IsGround() { return mbGround; }

	
		void SetNeedGravity(bool isAffected, Vector3 gravity = Vector3(0,0,0))
		{
			if (isAffected == false)
				mbIsAffectedByGravity = false;
			
			if (isAffected == true)
				mbIsAffectedByGravity = true;
				
			mGravity = gravity;

		}	

		void ClearForces();



		
	private:
	
		bool mbGround;
		bool mbIsAffectedByGravity;

		float mMass;
		float mFriction;
		float mAirDrag;
		float gravity;
		
		Vector3 mAccelation;
		Vector3 mForce;
		Vector3 mVelocity;
		Vector3 mLimitVelocity;
		Vector3 mGravity;

		eForceMode curForceMode;

		Vector3 sumForce{ 0,0,0 }, sumAccel{ 0,0,0 }, sumImpulse{ 0,0,0 }, sumVelChange{ 0,0,0 };

	};

}
