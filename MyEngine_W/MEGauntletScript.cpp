#include "MEGauntletScript.h"
#include "../MyEngine_Source/MEAnimator3D.h"
#include "../MyEngine_Source/MEInput.h"
#include "../MyEngine_Source/MEBoxCollider3D.h"
#include "MEActorScript.h"

namespace ME
{
	GauntletScript::GauntletScript()

	{
	}
	GauntletScript::~GauntletScript()
	{

	}
	void GauntletScript::Initialize()
	{
		WeaponScript::Initialize();
		mWeaponType = eWeaponType::Gauntlet;
	}

	void GauntletScript::OnRegister(ActorScript* ownerActor)
	{
		if (ownerActor == nullptr)
		{
			return;
		}
		mActorScript = ownerActor;
		Animator3D* animator = ownerActor->GetAnimator();

		if (animator == nullptr)
		{
			ownerActor->SetAnimator();
			animator = ownerActor->GetAnimator();
		}


		animator->AddEvent(L"MONSTER_ATTACK", L"HitBox On", 0.1f, [this]() {
			this->BeginAttack();
			});
		animator->AddEvent(L"MONSTER_ATTACK", L"HitBox Off", 0.6f, [this]() {
			this->EndAttack();
			});

		animator->AddEvent(L"MONSTER_ATTACK2", L"HitBox On", 0.25f, [this]() {
			this->BeginAttack();
			});
		animator->AddEvent(L"MONSTER_ATTACK2", L"HitBox Off", 0.7f, [this]() {
			this->EndAttack();
			});

		animator->AddEvent(L"MONSTER_ATTACK3", L"HitBox On", 0.3f, [this]() {
			this->BeginAttack();
			});
		animator->AddEvent(L"MONSTER_ATTACK3", L"HitBox Off", 0.8f, [this]() {
			this->EndAttack();
			}); 
		
		WeaponScript::OnRegister(ownerActor);
	}

	void GauntletScript::Update()
	{

		if (mWeaponTransform == nullptr)
		{
			mWeaponTransform = GetOwner()->GetComponent<Transform>();
		}


	}
	void GauntletScript::LateUpdate()
	{

		GauntletScript::UpdateWeaponTransform();

	//	auto* col = GetOwner()->GetComponent<BoxCollider3D>();
	//	Vector3 offset = col->GetOffset();
	//	if (Input::GetKey(eKeyCode::I)) mOffsetPos.z += 1.0f;
	//	if (Input::GetKey(eKeyCode::K)) mOffsetPos.z -= 1.0f;
	//	if (Input::GetKey(eKeyCode::J)) mOffsetPos.x -= 1.0f;
	//	if (Input::GetKey(eKeyCode::L)) mOffsetPos.x += 1.0f;
	//	if (Input::GetKey(eKeyCode::P)) mOffsetPos.y += 1.0f;
	//	if (Input::GetKey(eKeyCode::U)) mOffsetPos.y -= 1.0f;
	//
	//	col->SetOffset(offset);
	//
	//	OutputDebugStringA((std::string("Collider Offset: ") + std::to_string(mOffsetPos.x) + ", " + std::to_string(mOffsetPos.y) + ", " + std::to_string(mOffsetPos.z) + "\n").c_str());
	}
	void GauntletScript::Render()
	{
	}

	bool GauntletScript::Use(WeaponAttackInfo& outAttackInfo) 
	{
		// 칼을 휘두를 때 작동할 로직을 여기에 작성합니다.
		// (예: 칼의 콜라이더 판정 켜기, 검기 이펙트 생성, 효과음 재생 등)
	//	mActorScript->mAnimator->PlayAnimation(L"SWORDSLASH1", false);
		return true;
	}
}