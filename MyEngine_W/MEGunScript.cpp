#include "MEGunScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"
#include "Bullet.h"
#include "MEObject.h"
#include "MEBulletScript.h"
#include "MEModel.h"
#include "MEModelRenderer.h"
#include "MEResources.h"
#include "MEMaterial.h"
#include "METexture.h"
#include "MERigidbody.h"
#include "MEBoxCollider3D.h"
#include "MEActorScript.h"


namespace ME
{
	GunScript::GunScript()
		:mCoolDownTime(0.2f)
		, mCoolDownTimer(0.0f)
		, mbCanShoot(false)

	{
	}
	GunScript::~GunScript()
	{
		
	}
	void GunScript::Initialize()
	{
		WeaponScript::Initialize();
		mOffsetPos = Vector3(116.0f, 192.0f, 22.0f);
		mOffsetRot = Vector3(90.0f, 90.0f, 0.0f);
		mBulletPool = std::make_unique<ObjectPool<GameObject>>(50, [this]()->GameObject*
		{
			return this->makeBullet();
		});
	}
	void GunScript::Update()
	{
	
		if (mWeaponTransform == nullptr)
		{
			mWeaponTransform = GetOwner()->GetComponent<Transform>();
		}

		adjustGunPos();
	

		if (!mbCanShoot) {
			mCoolDownTimer += Time::DeltaTime();
			if (mCoolDownTimer > mCoolDownTime) {
				mbCanShoot = true;
				mCoolDownTimer = 0.0f;
			}
		}

		if (mOwnerType == OwnerType::Enemy && mbCanShoot) {
			Fire();
	
			mCoolDownTime = (float)(rand() % 3 + 2);
		}

	}
	void GunScript::LateUpdate()
	{
		//adjustGunPos();
	}
	void GunScript::Render()
	{
	}

	void GunScript::Fire()
	{
		if (mbCanShoot)
		{
			shootBullet();
			mbCanShoot = false;
			mCoolDownTimer = 0.0f;
		}
	}

	void GunScript::adjustGunPos()
	{
		WeaponScript::UpdateWeaponTransform();

		
		Vector3 forwardDir = mActorScript->GetAimDirection();
		forwardDir.y = 0.0f;
		forwardDir.Normalize();

		if (forwardDir.LengthSquared() > 0.0001f)
		{
			float yawRad = atan2f(forwardDir.x, forwardDir.z);
			float yawDeg = XMConvertToDegrees(yawRad);

			Vector3 rt = mWeaponTransform->GetRotation();
			rt.y = yawDeg;
			mWeaponTransform->SetRotation(rt);
		}
	
	
	}

	GameObject* GunScript::makeBullet()
	{
		if (mGunTransform == nullptr)
			return nullptr;

		Transform* tr = mGunTransform;
		Vector3 gunPos = tr->GetPosition();
	

		Vector3 spawnPos = gunPos;
		Bullet* bullet = object::Instantiate<Bullet>(enums::eLayerType::Bullet, spawnPos);
		BulletScript* bulletScript = bullet->AddComponent<BulletScript>();
		bulletScript->SetPool(mBulletPool.get());

		BoxCollider3D* col = bullet->AddComponent<BoxCollider3D>();
		col->SetSize(Vector3(3, 2, 2));
		
		Rigidbody* rb = bullet->AddComponent<Rigidbody>();
		rb->SetMass(2.0f);

		Transform* bulletTr = bullet->GetComponent<Transform>();
		bulletTr->SetScale(.1f, .1f, .1f);
		bulletTr->SetRotation(tr->GetRotation());


		std::shared_ptr<Model> bullet_model = Resources::Find<Model>(L"BulletModel");

		if (bullet_model !=  nullptr)
		{

			ModelRenderer* modelRenderer = bullet->AddComponent<ModelRenderer>();
			modelRenderer->SetMesh(bullet_model->GetMeshes());

			bullet->SetModel(bullet_model);

		}
		else
		{
			bullet_model = nullptr;
		}

		return static_cast<GameObject*>(bullet);

	}

	void GunScript::shootBullet()
	{
		auto bullet = mBulletPool->Get();

		if (bullet == nullptr) return;
		
		if (mGunTransform == nullptr)
			mGunTransform = GetOwner()->GetComponent<Transform>();
		
		Vector3 gunPos = mGunTransform->GetPosition();

		Transform* bulletTr = bullet->GetComponent<Transform>();


		Rigidbody* rb = bullet->GetComponent<Rigidbody>();

		bulletTr->SetPosition(gunPos);
		bulletTr->SetRotation(mGunTransform->GetRotation());
		
		Vector3 dir = mGunTransform->Forward();
		dir.Normalize();

		dir = (dir + mGunTransform->Up() * 0.0001f);
		dir.Normalize();
		rb->SetLimitVelocity(Vector3(5000.0f, 1000.0f, 5000.0f));
		rb->AddForce(dir * 2000.0f, Rigidbody::eForceMode::Impulse);
	}	
}