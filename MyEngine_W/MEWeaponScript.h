#pragma once
#include "MEScript.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MEActorScript.h"

namespace ME
{
	class ActorScript;
	class Bone;
	class WeaponScript : public Script	
	{
	public:

		enum class OwnerType
		{
			Player,
			Enemy,
			End,
		};

		virtual void Use() = 0;


		virtual void SetOnwer(GameObject* owner)
		{
			mOwner = owner;

			if (mOwner)
			{
				mOwnerTransform = mOwner->GetComponent<Transform>();
				mWeaponTransform = GetOwner()->GetComponent<Transform>();
				mActorScript = mOwner->GetComponent<ActorScript>();
				mActorScript->SetWeaponEquipment(this);
			}
		}

		virtual Bone* GetSocketBone();


		GameObject* GetOwnerActor() { return mOwner; }

		void SetIsAttackEnd(bool end) { mbIsAttackEnd = end; }
		bool GetIsAttackEnd() { return mbIsAttackEnd; }

	protected:

		virtual void UpdateWeaponTransform();

	private:		
		void AttachToSocket();

	protected :

		GameObject* mOwner;
		Transform* mWeaponTransform;
		Transform* mOwnerTransform;
		ActorScript* mActorScript;
		OwnerType mOwnerType;

		bool mbIsAttackEnd = false;

	public :
		math::Quaternion mOffsetQuat;
		math::Vector3 mOffsetPos;

		std::wstring mIdleAnimName;
		std::wstring mWalkAnimName;
		std::wstring mAttackAnimName;

	
	};

}
