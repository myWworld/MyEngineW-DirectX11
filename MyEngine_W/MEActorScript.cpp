#include "MEActorScript.h"
#include "MEWeaponScript.h"

namespace ME
{
	 void ActorScript::AddWeapon(WeaponScript* weapon)
	{
		if (weapon == nullptr) return;

		mInventoryWeapons.push_back(weapon);

		// 만약 처음 얻은 무기라면 바로 장착
		if (mInventoryWeapons.size() == 1)
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
		 if (mInventoryWeapons.empty() || index < 0 || index >= mInventoryWeapons.size()) return;

		 // 기존 무기 비활성화 (숨기기)
		 if (mCurrentWeaponIndex != -1 && mEquippedWeapon != nullptr)
		 {
			 mEquippedWeapon->GetOwner()->SetActive(false);
		 }

		 // 새 무기 장착
		 mCurrentWeaponIndex = index;
		 mEquippedWeapon = mInventoryWeapons[index];
		 mEquippedWeapon->GetOwner()->SetActive(true); // 화면에 보이게 켬

		 if (!mEquippedWeapon->IsEventsRegistered())
		 {
			 mEquippedWeapon->OnRegister();
		 }

		 mbHoldingWeapon = true;
	 }


	 void ActorScript::AttachActiveWeapon(WeaponScript* weapon)
	 {
		 if (weapon == nullptr) return;

		 mActiveWeapons.push_back(weapon);
		 weapon->GetOwner()->SetActive(true); 
		 mbHoldingWeapon = true;
	 }

	 void ActorScript::SetHP(float damage)
	 {
		 mHp -= damage;
		 if (mHp <= 0.0f)
		 {
			 mHp = 0.0f;
			 OnDeath();
		 }
	 }


}