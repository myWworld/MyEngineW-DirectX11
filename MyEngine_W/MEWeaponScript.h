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

	public :
		math::Vector3 mOffsetRot;
		math::Vector3 mOffsetPos;
	};

}
