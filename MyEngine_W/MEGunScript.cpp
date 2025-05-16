#include "MEGunScript.h"
#include "METransform.h"
#include "MEInput.h"
#include "MEGameObject.h"
#include "METime.h"
#include "MEAnimator3D.h"
#include "MERenderer.h"

namespace ME
{
	GunScript::GunScript()
	{
	}
	GunScript::~GunScript()
	{
	}
	void GunScript::Initialize()
	{
	}
	void GunScript::Update()
	{
		if (mPlayerScript->IsUsingGun())
		{
			Transform* tr = GetOwner()->GetComponent<Transform>();
			Bone* RightHandBone = mPlayerScript->GetRightHandBone();

			Matrix finalMatrix = RightHandBone->FinalTransform;

			Vector3 position;
			Quaternion rotation;
			Vector3 scale;

			finalMatrix.Decompose(scale, rotation, position);

			tr->SetPosition(position);
			tr->SetRotation(rotation.ToEuler());
			tr->SetScale(scale);
		}
		
	}
	void GunScript::LateUpdate()
	{
	}
	void GunScript::Render()
	{
	}
}