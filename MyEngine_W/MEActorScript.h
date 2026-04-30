#pragma once
#include "MEScript.h"

namespace ME
{
	class ActorScript: public Script
	{

	public:
		

		virtual void OnPrimaryAction() = 0;
		virtual void OnToggleWeapon()
		{
			mbHoldingGun = !mbHoldingGun;
		}

	protected:
		bool mbHoldingGun = false;

	};
}

