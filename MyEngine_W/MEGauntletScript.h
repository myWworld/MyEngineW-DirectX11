#pragma once
#include "MEWeaponScript.h"

namespace ME
{
	class GauntletScript : public WeaponScript
	{
	public:

		GauntletScript();
		~GauntletScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		void Use() override;

		void OnRegister() override;


	};

}

