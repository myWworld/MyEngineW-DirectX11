#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "MEMath.h"
#include "MECamera.h"

#include "MEPlayer.h"
#include "MEPlayerScript.h"
#include "MEEnemyScript.h"

namespace ME
{


	class GunScript :public Script
	{
	public:

		enum class PlayerType
		{
			Player,
			Enemy,
			End,
		};

		GunScript();
		~GunScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		void SetGunOnwer(Player* player, bool bIsEnemy = false)
		{
			mOwner = player;

			if (bIsEnemy == false)
			{
				mPlayerType = PlayerType::Player;
				mPlayerScript = mOwner->GetComponent<PlayerScript>();
			}
			else
			{
				mPlayerType = PlayerType::Enemy;
				mEnemyScript = mOwner->GetComponent<EnemyScript>();
			}
		
		}


		Player* GetGunOwner() { return mOwner; }

	private:

		void makeBullet();

	private:


		 Player* mOwner;
		 PlayerScript * mPlayerScript;
		 EnemyScript* mEnemyScript;

		 PlayerType mPlayerType;

		 Vector3 mPrevPlayerPos = Vector3::Zero;
		 Vector3 mCurPlayerPos = Vector3::Zero;

		 float mCoolDownTime;
		 float mCoolDownTimer;

		 bool mbCanShoot;
	};

}
