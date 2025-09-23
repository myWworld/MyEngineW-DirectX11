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
	}
	void GunScript::Update()
	{
		
		if (mOwner != nullptr)
		{
			Transform* playerTr = GetGunOwner()->GetComponent<Transform>();

			mCurPlayerPos = playerTr->GetPosition();
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

			if (Input::GetKeyDown(eKeyCode::LeftMous))
			{
				if (mbCanShoot == true && mPlayerScript->IsUsingGun())
				{
					makeBullet();
					mbCanShoot = false;
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
				makeBullet();
				mbCanShoot = false;
			}
		}
	}
	void GunScript::LateUpdate()
	{
		if (mPlayerType == PlayerType::Player)
		{
			if (mPlayerScript->IsUsingGun())
			{
				Transform* tr = GetOwner()->GetComponent<Transform>();
				Transform* playerTr = GetGunOwner()->GetComponent<Transform>();

				Vector3 moveOffset = mCurPlayerPos - mPrevPlayerPos;


				Bone* leftHand = mPlayerScript->GetLeftHandBone();

				Matrix handLocal = leftHand->FinalTransform;

				Matrix playerWorldMatrix = playerTr->GetWorldMatrix();

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


				Vector3 camForward = renderer::mainCamera->GetForward();
				camForward.y = 0.0f;
				camForward.Normalize();

				if (camForward.LengthSquared() > 0.0001f)
				{
					float yawRad = atan2f(camForward.x, camForward.z);
					float yawDeg = XMConvertToDegrees(yawRad);

					Vector3 rt = tr->GetRotation();
					rt.y = yawDeg;
					tr->SetRotation(rt);
				}

				Vector3 pos;
				Quaternion rot;
				Vector3 scale;
				worldMatrix.Decompose(scale, rot, pos);
				tr->SetPosition(pos);
				//tr->SetRotation(rot.ToEuler());
				scale = tr->GetScale();
				tr->SetScale(scale);

			}

			mPrevPlayerPos = mCurPlayerPos;
		}
		else if(mPlayerType == PlayerType::Enemy)
		{
			if (mEnemyScript->IsUsingGun())
			{
				Transform* tr = GetOwner()->GetComponent<Transform>();
				Transform* playerTr = GetGunOwner()->GetComponent<Transform>();

				Vector3 moveOffset = mCurPlayerPos - mPrevPlayerPos;


				Bone* leftHand = mEnemyScript->GetLeftHandBone();

				Matrix handLocal = leftHand->FinalTransform;

				Matrix playerWorldMatrix = playerTr->GetWorldMatrix();

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

				Vector3 charForward = mOwner->GetComponent<Transform>()->Forward();
				charForward.y = 0.0f;
				charForward.Normalize();

				if (charForward.LengthSquared() > 0.0001f)
				{
					float yawRad = atan2f(charForward.x, charForward.z);
					float yawDeg = XMConvertToDegrees(yawRad);

					Vector3 rt = tr->GetRotation();
					rt.y = yawDeg;
					tr->SetRotation(rt);
				}

				Vector3 pos;
				Quaternion rot;
				Vector3 scale;
				worldMatrix.Decompose(scale, rot, pos);
				tr->SetPosition(pos);
				//tr->SetRotation(rot.ToEuler());
				scale = tr->GetScale();
				tr->SetScale(scale);
			}
		}
		
	}
	void GunScript::Render()
	{
	}

	void GunScript::makeBullet()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 gunPos = tr->GetPosition();
	

		Vector3 spawnPos = gunPos;
		Bullet* bullet = object::Instantiate<Bullet>(enums::eLayerType::Particle,spawnPos);
		bullet->AddComponent<BulletScript>();
		
		Rigidbody* rb = bullet->AddComponent<Rigidbody>();
		rb->SetMass(2.0f);

		Transform* bulletTr = bullet->GetComponent<Transform>();
		bulletTr->SetScale(.1f, .1f, .1f);
		bulletTr->SetRotation(tr->GetRotation());

		Model* bullet_model = new Model();

		if (bullet_model->LoadModel(L"..\\Resources\\Bullet.fbx"))
		{

			ModelRenderer* modelRenderer = bullet->AddComponent<ModelRenderer>();
			modelRenderer->SetMesh(bullet_model->GetMeshes());
			modelRenderer->SetMaterial(Resources::Find<Material>(L"StaticModelMaterial"));

			if (bullet_model->GetTextures().size() > 0)
				modelRenderer->SetTextures(bullet_model->GetTextures());
			else
				modelRenderer->SetTexture(Resources::Find<graphics::Texture>(L"PISTOL"));

			bullet->SetModel(bullet_model);

			Vector3 dir = tr->Forward();
			dir.Normalize();
			dir = (dir + tr->Up() * 0.01f);
			dir.Normalize();
			rb->AddForce(dir * 5000.0f, Rigidbody::eForceMode::Impulse);
		}
		else
		{
			delete bullet_model;
			bullet_model = nullptr;
		}

	}
}