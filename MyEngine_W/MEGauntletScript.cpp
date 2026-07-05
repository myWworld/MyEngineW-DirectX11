#include "MEGauntletScript.h"
#include "../MyEngine_Source/MEAnimator3D.h"
#include "../MyEngine_Source/MEInput.h"
#include "../MyEngine_Source/MEBoxCollider3D.h"

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

		mOffsetPos = Vector3(70.0f, 140.0f, -10.0f); //x +로 갈 시 아래쪽으로 감 , y축 +시 왼쪽으로 이동, z축은 앞쪽같음
		Vector3 offsetRotDegree = Vector3(90.0f, 180.0f, 15.0f);

		float pitch = XMConvertToRadians(offsetRotDegree.x);
		float yaw = XMConvertToRadians(offsetRotDegree.y);
		float roll = XMConvertToRadians(offsetRotDegree.z);

		mOffsetQuat = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);

		mIdleAnimName = L"SWORDIDLE1";
		mWalkAnimName = L"SWORDWALK";
		mAttackAnimName = L"SWORDATTACK1";
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

		auto* col = GetOwner()->GetComponent<BoxCollider3D>();
		Vector3 offset = col->GetOffset();
		if (Input::GetKey(eKeyCode::I)) offset.z += 1.0f;
		if (Input::GetKey(eKeyCode::K)) offset.z -= 1.0f;
		if (Input::GetKey(eKeyCode::J)) offset.x -= 1.0f;
		if (Input::GetKey(eKeyCode::L)) offset.x += 1.0f;
		if (Input::GetKey(eKeyCode::P)) offset.y += 1.0f;
		if (Input::GetKey(eKeyCode::U)) offset.y -= 1.0f;

		col->SetOffset(offset);

		//OutputDebugStringA((std::string("Collider Offset: ") + std::to_string(offset.x) + ", " + std::to_string(offset.y) + ", " + std::to_string(offset.z) + "\n").c_str());
	}
	void GauntletScript::Render()
	{
	}

	void GauntletScript::Use()
	{
		// 칼을 휘두를 때 작동할 로직을 여기에 작성합니다.
		// (예: 칼의 콜라이더 판정 켜기, 검기 이펙트 생성, 효과음 재생 등)
		mActorScript->mAnimator->PlayAnimation(L"SWORDSLASH1", false);

	}
}