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
		
		if (mOwner != nullptr)
		{
			Transform* playerTr = GetGunOwner()->GetComponent<Transform>();

			mCurPlayerPos = playerTr->GetPosition();
		}
	}
	void GunScript::LateUpdate()
	{
		if (mPlayerScript->IsUsingGun())
		{
			Transform* tr = GetOwner()->GetComponent<Transform>();
			Transform* playerTr = GetGunOwner()->GetComponent<Transform>();

			Vector3 moveOffset = mCurPlayerPos - mPrevPlayerPos;


			Bone* leftHand = mPlayerScript->GetLeftHandBone();

			Matrix handLocal = leftHand->FinalTransform;

			Matrix playerWorldMatrix = playerTr->GetWorldMatrix();
			
			Matrix handMatrix = handLocal * playerWorldMatrix;
			// 손 위치 & 회전 추출
			Vector3 handPos = handMatrix.Translation();
			Quaternion handRot = Quaternion::CreateFromRotationMatrix(handMatrix);
			
			
			// 최종 위치 & 회전
			Vector3 gunPos = handPos;
			Quaternion gunRot = handRot;
			
			Vector3 offset = Vector3(96.0f, 152.0f, 22.0f);
			Vector3 finalPos = gunPos + Vector3::Transform(offset, Matrix::CreateFromQuaternion(gunRot));
			
			Matrix worldMatrix = Matrix::CreateFromQuaternion(gunRot) * Matrix::CreateTranslation(finalPos);
			

			Vector3 camForward = renderer::mainCamera->GetForward();
			camForward.y = 0.0f;
			camForward.Normalize();

			if (camForward.LengthSquared() > 0.0001f)
			{
				float yawRad = atan2f(camForward.x, camForward.z);
				float yawDeg = XMConvertToDegrees(yawRad);

				Vector3 rt = tr->GetRotation();
				rt.y = yawDeg;
				tr->SetRotation(rt);
			}
			
			Vector3 pos;
			Quaternion rot;
			Vector3 scale;
			worldMatrix.Decompose(scale, rot, pos);
			tr->SetPosition(pos);
			//tr->SetRotation(rot.ToEuler());
			scale = tr->GetScale();
			tr->SetScale(scale);

		}

		mPrevPlayerPos = mCurPlayerPos;
	}
	void GunScript::Render()
	{
	}
}