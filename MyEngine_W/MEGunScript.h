#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "MEMath.h"
#include "MECamera.h"

#include "MEPlayer.h"
#include "MEPlayerScript.h"

namespace ME
{


	class GunScript :public Script
	{
	public:


		GunScript();
		~GunScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		void SetGunOnwer( Player* player) 
		{ mOwner = player; 
			mPlayerScript = mOwner->GetComponent<PlayerScript>();
		}

		Player* GetGunOwner() { return mOwner; }

	private:

		void makeBullet();

	private:


		 Player* mOwner;
		 PlayerScript * mPlayerScript;

		 Vector3 mPrevPlayerPos = Vector3::Zero;
		 Vector3 mCurPlayerPos = Vector3::Zero;

		 float mCoolDownTime;
		 float mCoolDownTimer;

		 bool mbCanShoot;
	};

}
