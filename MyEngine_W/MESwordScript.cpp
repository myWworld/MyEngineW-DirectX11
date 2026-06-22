#include "MESwordScript.h"

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

		mOffsetPos = Vector3(50.0f, 85.0f, -100.0f); //x +로 갈 시 아래쪽으로 감 , y축 +시 왼쪽으로 이동, z축은 앞쪽같음
		mOffsetRot = Vector3(-90.0f, 0.0f, 0.0f);
	}
	void SwordScript::Update()
	{

		if (mWeaponTransform == nullptr)
		{
			mWeaponTransform = GetOwner()->GetComponent<Transform>();
		}

		WeaponScript::UpdateWeaponTransform();
	}
	void SwordScript::LateUpdate()
	{
		
		//adjustGunPos();
	}
	void SwordScript::Render()
	{
	}

	void SwordScript::Use()
	{
		// 칼을 휘두를 때 작동할 로직을 여기에 작성합니다.
		// (예: 칼의 콜라이더 판정 켜기, 검기 이펙트 생성, 효과음 재생 등)
	}
}