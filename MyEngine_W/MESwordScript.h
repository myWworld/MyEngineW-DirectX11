#pragma once
#include "MEWeaponScript.h"

namespace ME
{
class SwordScript : public WeaponScript
{
public:

	SwordScript();
	~SwordScript();

	void Initialize() override;
	void Update()override;
	void LateUpdate()override;
	void Render()override;	

	void Use() override;



};

}

