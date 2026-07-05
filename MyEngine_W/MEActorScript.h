#pragma once
#include "MEScript.h"
#include "MEDamageInfo.h"
#include <vector>

namespace ME
{
	class Bone;
	class WeaponScript;
	class Animator3D;
	class ActorScript: public Script
	{

	public:
		float GetHP() { return mHp; }
		void SetHP(float damage);
		Animator3D* GetAnimator() { return mAnimator; }
		

		virtual void OnPrimaryAction() = 0;
		virtual Bone* GetWeaponSocketBone() = 0;
		virtual Bone* GetWeaponSocketBoneRight() = 0;
		virtual Vector3 GetAimDirection() = 0;

		virtual void OnDeath() = 0;
		virtual void DamageProcess(DamageInfo info) = 0;

		bool IsUsingWeapon() { return mbHoldingWeapon; }	

		virtual void AddWeapon(WeaponScript* weapon);
		virtual void EquipWeapon(int index);
		virtual void OnToggleWeapon()
		{
			if (mInventoryWeapons.size() <= 1) return;

			int nextIdx = (mCurrentWeaponIndex + 1) % mInventoryWeapons.size();
			EquipWeapon(nextIdx);
		}

		virtual void SetWeaponEquipment(WeaponScript* weapon)
		{
			mEquippedWeapon = weapon;
		}
		
		virtual void AttachActiveWeapon(WeaponScript* weapon);


	protected:
		bool mbHoldingWeapon = false;

		std::vector<WeaponScript*> mInventoryWeapons; // 가방에 있는 무기들 (플레이어용)
		std::vector<WeaponScript*> mActiveWeapons;
		int mCurrentWeaponIndex = -1;        // 현재 들고 있는 무기의 배열 번호
		WeaponScript* mEquippedWeapon;       // 현재 들고 있는 무기에 빠르게 접근하기 위한 캐싱 포인터


		float mHp = 100.0f;

	public:
		Animator3D* mAnimator = nullptr;

	};
}

