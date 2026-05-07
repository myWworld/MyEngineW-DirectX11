#include "MEWeaponScript.h"
#include "MEBone.h"
#include "MEActorScript.h"

namespace ME
{
	Bone* WeaponScript::GetSocketBone()
	{
		if (!mActorScript) return nullptr;

		return mActorScript->GetWeaponSocketBone();
	}
}