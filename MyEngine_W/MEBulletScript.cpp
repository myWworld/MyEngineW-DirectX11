#include "MEBulletScript.h"
#include "METime.h"
#include "MEGameObject.h"
#include "METransform.h"

namespace ME
{
	BulletScript::BulletScript()
		:mDeadTime(8.0f)
		,mDeadTimer(0.0f)
	{
	}
	BulletScript::~BulletScript()
	{
	}
	void BulletScript::Initialize()
	{
	}
	void BulletScript::Update()
	{
		mDeadTimer += Time::DeltaTime();

		//Transform* tr = GetOwner()->GetComponent<Transform>();
		//Vector3 pos = tr->GetPosition();
		//
		//pos += tr->Forward() * 4.0f;
		//
		//tr->SetPosition(pos);


		if (mDeadTime < mDeadTimer)
		{
			mDeadTimer = 0.0f;
			GetOwner()->SetDeath();
		}
	}
	void BulletScript::LateUpdate()
	{
	}
	void BulletScript::Render()
	{
	}
}
