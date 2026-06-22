#include "MECamera.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MEApplication.h"
#include "MEFrustumCulling.h"

extern ME::Application application;

namespace ME
{
	Matrix Camera::ViewMatrix = Matrix::Identity;
	Matrix Camera::ProjectionMatrix = Matrix::Identity;

	Camera::Camera()
		:Component(enums::eComponentType::Camera)
		,mProjectionType(eProjectionType::Perspective)
		,mProjectionMatrix(Matrix::Identity)
		,mViewMatrix(Matrix::Identity)
		,mAspectRatio(0.0f)
		,mNear(1.0f)
		,mFar(20000.0f)
		,mSize(1.0f)
		,mCameraPitch(0.0f)
		, mCameraRoll(0.0f)
		, mCameraYaw(0.0f)
		, mForward(Vector3::Forward)
		, mFrustumCulling(std::make_unique<FrustumCulling>())
	{
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		
	}

	void Camera::Update()
	{	
		if (mCamScript == nullptr)
		{
			mCamScript = GetOwner()->GetComponent<CameraScript>();
		}
	}

	void Camera::LateUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix(mProjectionType);

		ViewMatrix = mViewMatrix;
		ProjectionMatrix = mProjectionMatrix;

	}
	void Camera::Render()
	{
	}

	void Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Vector3 forward = tr->Forward();
		const Vector3 up = tr->Up();

		if (mtarget != nullptr)
		{
			mTargetPos = mtarget->GetComponent<Transform>()->GetPosition();
			
			Transform* camTr = GetOwner()->GetComponent<Transform>();

			mCameraPitch = std::clamp(mCameraPitch, XMConvertToRadians(-89.0f), XMConvertToRadians(89.0f));

			Matrix yawRot = Matrix::CreateRotationY(mCameraYaw);
			Vector3 offset = Vector3::Zero; // 높이는 고정
			
			if (mCamScript->GetPerspective() == CameraScript::perspective::ThirdPerson)
			{
				offset = Vector3(0.0f, 150, -100);
				mFovY = 75.0f;
			}
			else
			{
				mFovY = 75.0f;
				offset = Vector3(0.0f, 200, 45);
			}
		
			Vector3 camPos = mTargetPos + Vector3::Transform(offset, yawRot);

			// 그다음 pitch 포함한 회전으로 forward 구해서 target 지정
			Matrix fullRot = Matrix::CreateRotationX(mCameraPitch) * Matrix::CreateRotationY(mCameraYaw);
			Vector3 camForward = Vector3::Transform(Vector3::Forward, fullRot);
			mForward = camForward;
			mRight = Vector3::Transform(Vector3::Right, fullRot);

			Vector3 camTarget = camPos + camForward;
			
			Quaternion rot = Quaternion::CreateFromRotationMatrix(fullRot);

			// Transform에 회전 적용
			tr->SetRotation(rot.ToEuler());
			tr->SetPosition(camPos);

			mViewMatrix = Matrix::CreateLookAtLH(camPos, camTarget, Vector3::Up);

			return;
		}
		
		

		mViewMatrix = Matrix::CreateLookToLH(pos, forward, up);
	}

	void Camera::CreateProjectionMatrix(eProjectionType type)
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = (winRect.right - winRect.left);
		float height = (winRect.bottom - winRect.top);
		mAspectRatio = width / height;
		float fovY = XMConvertToRadians(mFovY);

		switch (type)
		{
		case eProjectionType::Perspective:
			mProjectionMatrix = Matrix::CreatePerspectiveFieldOfViewLH(fovY, mAspectRatio, mNear, mFar);
			break;
		case eProjectionType::Orthographic:
			mProjectionMatrix = Matrix::CreateOrthographicLH(width / mSize, height / mSize, mNear, mFar);
			break;
		}


	}
}