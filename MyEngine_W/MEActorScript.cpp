#include "MEActorScript.h"
#include "MEWeaponScript.h"

namespace ME
{
	 void ActorScript::AddWeapon(WeaponScript* weapon)
	{
		if (weapon == nullptr) return;

		mWeapons.push_back(weapon);

		// 만약 처음 얻은 무기라면 바로 장착
		if (mWeapons.size() == 1)
		{
			EquipWeapon(0);
		}
		else
		{
			// 장착하지 않은 무기는 화면에서 안 보이게 함
			weapon->GetOwner()->SetActive(false);
		}
	}

	 void ActorScript::EquipWeapon(int index)
	 {
		 if (mWeapons.empty() || index < 0 || index >= mWeapons.size()) return;

		 // 기존 무기 비활성화 (숨기기)
		 if (mCurrentWeaponIndex != -1 && mEquippedWeapon != nullptr)
		 {
			 mEquippedWeapon->GetOwner()->SetActive(false);
		 }

		 // 새 무기 장착
		 mCurrentWeaponIndex = index;
		 mEquippedWeapon = mWeapons[index];
		 mEquippedWeapon->GetOwner()->SetActive(true); // 화면에 보이게 켬

		 mbHoldingWeapon = true;
	 }


}