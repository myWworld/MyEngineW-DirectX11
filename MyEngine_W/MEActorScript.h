#pragma once
#include "MEScript.h"
#include <vector>

namespace ME
{
	class Bone;
	class WeaponScript;
	class ActorScript: public Script
	{

	public:
		

		virtual void OnPrimaryAction() = 0;
		virtual Bone* GetWeaponSocketBone() = 0;
		virtual Vector3 GetAimDirection() = 0;
		bool IsUsingWeapon() { return mbHoldingWeapon; }

		virtual void AddWeapon(WeaponScript* weapon);
		virtual void EquipWeapon(int index);
		virtual void OnToggleWeapon()
		{
			if (mWeapons.size() <= 1) return;

			int nextIdx = (mCurrentWeaponIndex + 1) % mWeapons.size();
			EquipWeapon(nextIdx);
		}

		virtual void SetWeaponEquipment(WeaponScript* weapon)
		{
			mEquippedWeapon = weapon;
		}

	protected:
		bool mbHoldingWeapon = false;

		std::vector<WeaponScript*> mWeapons; // 무기 인벤토리
		int mCurrentWeaponIndex = -1;        // 현재 들고 있는 무기의 배열 번호
		WeaponScript* mEquippedWeapon;       // 현재 들고 있는 무기에 빠르게 접근하기 위한 캐싱 포인터

	public:
		class Animator3D* mAnimator = nullptr;

	};
}

