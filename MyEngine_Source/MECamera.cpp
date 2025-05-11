#include "MECamera.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MEApplication.h"

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
		,mFar(1000.0f)
		,mSize(1.0f)
	
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
			Transform* targetTr = mtarget->GetComponent<Transform>();
			Vector3 targetPos = targetTr->GetPosition();

			// 카메라 위치를 타겟 기준으로 오프셋
			pos = targetPos + Vector3(0.0f, 60.0f, -45.0f);
			tr->SetPosition(pos);

			// 타겟 방향 계산
			Vector3 forward = targetPos - pos;
			if (forward.LengthSquared() < 0.0001f)
				forward = Vector3::Forward;
			else
				forward.Normalize();
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
		float fovY = XMConvertToRadians(70.0f);

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