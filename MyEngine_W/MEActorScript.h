#pragma once
#include "MEScript.h"

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

		virtual void OnToggleWeapon()
		{
			mbHoldingWeapon = !mbHoldingWeapon;
		}

		virtual void SetWeaponEquipment(WeaponScript* weapon)
		{
			mEquippedWeapon = weapon;
		}

	protected:
		bool mbHoldingWeapon = false;
		WeaponScript* mEquippedWeapon;

	};
}

