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


namespace ME
{
	GunScript::GunScript()
		:mCoolDownTime(0.2f)
		, mCoolDownTimer(0.0f)
		, mCoolDownTimeForEnemy(2.0f)
		, mCoolDownTimerForEnemy(0.0f)
		, mbCanShoot(false)
		,mPlayerScript(nullptr)
		,mEnemyScript(nullptr)
	{
	}
	GunScript::~GunScript()
	{
	}
	void GunScript::Initialize()
	{
		mBulletPool = new ObjectPool<GameObject>(50, [this]()->GameObject*
		{
			return this->makeBullet();
		});
	}
	void GunScript::Update()
	{
	

		if (mOwner != nullptr)
		{
			if(mPlayerTransform == nullptr)
				mPlayerTransform = mOwner->GetComponent<Transform>();
		}

		if (mGunTransform == nullptr)
		{
			mGunTransform = GetOwner()->GetComponent<Transform>();
		}


		if (mPlayerTransform)
		{
			mCurPlayerPos = mPlayerTransform->GetPosition();
		}
		

		if (mPlayerType == PlayerType::Player)
		{

			if (mbCanShoot == false )
			{
				mCoolDownTimer += Time::DeltaTime();

				if (mCoolDownTimer > mCoolDownTime)
				{
					mbCanShoot = true;
					mCoolDownTimer = 0.0f;
				}
			}

		}
		else if (mPlayerType == PlayerType::Enemy)
		{
			if (mbCanShoot == false)
			{
				mCoolDownTimerForEnemy += Time::DeltaTime();

				if (mCoolDownTimerForEnemy > mCoolDownTimeForEnemy)
				{
					mbCanShoot = true;
					mCoolDownTimeForEnemy = rand() % 3 + 2;
					mCoolDownTimerForEnemy = 0.0f;
				}
			}

			if (mbCanShoot)
			{
				shootBullet();
				mbCanShoot = false;
			}
		}
	}
	void GunScript::LateUpdate()
	{
		
		adjustGunPos();

		
	}
	void GunScript::Render()
	{
	}

	void GunScript::Fire()
	{
		if (mbCanShoot == false) return;

		shootBullet();
		mbCanShoot = false;
	}

	void GunScript::adjustGunPos()
	{
		if (mPlayerTransform == nullptr || mGunTransform == nullptr) return;

		Bone* leftHand = nullptr;
		Vector3 forwardDir;

		if (mPlayerType == PlayerType::Player && mPlayerScript->IsUsingGun())
		{
			leftHand = mPlayerScript->GetLeftHandBone();
			forwardDir = renderer::mainCamera->GetForward();
			mPrevPlayerPos = mCurPlayerPos;

		}
		else if (mPlayerType == PlayerType::Enemy && mEnemyScript->IsUsingGun())
		{
			leftHand = mEnemyScript->GetLeftHandBone();
			forwardDir = mPlayerTransform->Forward();
		}
		else
		{
			return;
		}


		if (leftHand == nullptr) return;


		Matrix handLocal = leftHand->FinalTransform;

		Matrix playerWorldMatrix = mPlayerTransform->GetWorldMatrix();

		Matrix handMatrix = handLocal * playerWorldMatrix;
		// 손 위치 & 회전 추출
		Vector3 handPos = handMatrix.Translation();
		Quaternion handRot = Quaternion::CreateFromRotationMatrix(handMatrix);


		// 최종 위치 & 회전
		Vector3 gunPos = handPos;
		Quaternion gunRot = handRot;

		Vector3 offset = Vector3(96.0f, 152.0f, 22.0f);
		Vector3 finalPos = gunPos + Vector3::Transform(offset, Matrix::CreateFromQuaternion(gunRot));

		Matrix worldMatrix = Matrix::CreateFromQuaternion(gunRot) * Matrix::CreateTranslation(finalPos);

	
		forwardDir.y = 0.0f;
		forwardDir.Normalize();

		if (forwardDir.LengthSquared() > 0.0001f)
		{
			float yawRad = atan2f(forwardDir.x, forwardDir.z);
			float yawDeg = XMConvertToDegrees(yawRad);

			Vector3 rt = mGunTransform->GetRotation();
			rt.y = yawDeg;
			mGunTransform->SetRotation(rt);
		}

		Vector3 pos;
		Quaternion rot;
		Vector3 scale;
		worldMatrix.Decompose(scale, rot, pos);
		mGunTransform->SetPosition(pos);
		scale = mGunTransform->GetScale();
		mGunTransform->SetScale(scale);

	
	
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
		bulletScript->SetPool(mBulletPool);

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
			modelRenderer->SetMaterial(Resources::Find<Material>(L"StaticModelMaterial"));

			if (bullet_model->GetTextures().size() > 0)
				modelRenderer->SetTextures(bullet_model->GetTextures());
			else
				modelRenderer->SetTexture(Resources::Find<graphics::Texture>(L"PISTOL"));

			bullet->SetModel(bullet_model);

		}
		else
		{
			bullet_model = nullptr;
		}

		return dynamic_cast<GameObject*>(bullet);

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