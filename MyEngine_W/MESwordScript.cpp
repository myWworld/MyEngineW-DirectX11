#include "MESwordScript.h"
#include "../MyEngine_Source/MEAnimator3D.h"

namespace ME
{
	SwordScript::SwordScript()

	{
	}
	SwordScript::~SwordScript()
	{

	}
	void SwordScript::Initialize()
	{
		WeaponScript::Initialize();

		mOffsetPos = Vector3(70.0f,140.0f, -10.0f); //x +로 갈 시 아래쪽으로 감 , y축 +시 왼쪽으로 이동, z축은 앞쪽같음
		Vector3 offsetRotDegree = Vector3(90.0f, 180.0f, 15.0f);

		float pitch = XMConvertToRadians(offsetRotDegree.x);
		float yaw = XMConvertToRadians(offsetRotDegree.y);
		float roll = XMConvertToRadians(offsetRotDegree.z);

		 mOffsetQuat = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);

		 mIdleAnimName = L"SWORDIDLE1";
		 mWalkAnimName = L"SWORDWALK";
		 mAttackAnimName = L"SWORDATTACK1";
	}
	void SwordScript::Update()
	{

		if (mWeaponTransform == nullptr)
		{
			mWeaponTransform = GetOwner()->GetComponent<Transform>();
		}

	}
	void SwordScript::LateUpdate()
	{
		
		WeaponScript::UpdateWeaponTransform();
	}
	void SwordScript::Render()
	{
	}

	void SwordScript::Use()
	{
		// 칼을 휘두를 때 작동할 로직을 여기에 작성합니다.
		// (예: 칼의 콜라이더 판정 켜기, 검기 이펙트 생성, 효과음 재생 등)
		mActorScript->mAnimator->PlayAnimation(L"SWORDSLASH1", false);

	}
}