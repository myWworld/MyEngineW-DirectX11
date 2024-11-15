#include "MERigidbody.h"
#include "METime.h"
#include "METransform.h"
#include "MEGameObject.h"
namespace ME
{
	Rigidbody::Rigidbody()
		:Component(enums::eComponentType::Rigidbody)
		, mbGround(false)
		, mMass(1.0f)
		, mFriction(70.0f)
	, mForce(Vector2::Zero)
	, mVelocity(Vector2::Zero)
	, mLimitVelocity(Vector2(130.0f,800.0f))
	, mGravity(Vector2(0.0f, 800.0f))
	, mAccelation(Vector2::Zero)
		, mbIsAffectedByGravity(true)
	{


	}
	Rigidbody::~Rigidbody()
	{
	}
	void Rigidbody::Initialize()
	{
	}
	void Rigidbody::Update()
	{
		// f = ma
		mAccelation = mForce / mMass;

		mVelocity += mAccelation * Time::DeltaTime();		

		if (mbGround)
		{
			Vector2 gravity = mGravity;
			gravity.Normalize();

			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;
		
		}
		else
		{
			//����

			mVelocity += mGravity * Time::DeltaTime();
		}

		//�ִ�ӵ� ����


		Vector2 gravity = mGravity;
		gravity.Normalize();

		float dot = mVelocity.Dot(gravity);
		gravity = gravity * dot;

		Vector2 sideVelocity = mVelocity - gravity;

		if (mLimitVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitVelocity.y;
		}

		if (mLimitVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitVelocity.x;
		}

		mVelocity = gravity + sideVelocity;

		if (!(mVelocity == Vector2::Zero))
		{
			//�ӵ��� �ݴ�������� ������ ����
			Vector2 friction = -mVelocity;
			friction.Normalize();
			friction = friction* mFriction * mMass * Time::DeltaTime();

			//���������� ���� �ӵ� ���ҷ��� ���� �ӵ����� ū ���
			if (mVelocity.Length() <= friction.Length())
			{
				mVelocity = Vector2::Zero;
			}
			else
			{
				mVelocity += friction;
			}

			Transform* tr = GetOwner()->GetComponent<Transform>();
			Vector2 pos = tr->GetPosition();

			pos = pos + mVelocity * Time::DeltaTime();
			tr->SetPosition(pos);

			mForce = Vector2::One;
		}
		

	}
	void Rigidbody::LateUpdate()
	{
	}
	void Rigidbody::Render(HDC hdc)
	{
	}

	void Rigidbody::StopMoving()
	{
		mForce =  Vector2::One;
		mVelocity = Vector2::Zero;
	}
}