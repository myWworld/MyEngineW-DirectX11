#include "MECamera.h"
#include "MEGameObject.h"
#include "METransform.h"
#include "MEApplication.h"

extern ME::Application application;

namespace ME
{
	Camera::Camera()
		:Component(enums::eComponentType::Camera)
		, mDistance(Vector2::Zero)
		, mResolution(Vector2::Zero)
		, mLookPosition(Vector2::Zero)
		, mTarget(nullptr)
	{
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		mResolution.x = (float)application.GetWidth();
		mResolution.y = (float)application.GetHeight();
	}

	void Camera::Update()
	{
		if (mTarget)
		{
			Transform* tr = mTarget->GetComponent<Transform>();
			mLookPosition = tr->GetPosition();
			mLookPosition.y = mResolution.y / 2.0f;


			if(mTarget->GetComponent<Transform>()->GetName() == L"Player")
					mDistance = mLookPosition - Vector2(mResolution.x/2.0f,100);
			else
				mDistance = mLookPosition - (mResolution / 2.0f);
			
		}
		else
		{
			Transform* cameraTr = GetOwner()->GetComponent<Transform>();
			mLookPosition = cameraTr->GetPosition();
			mDistance = mLookPosition - (mResolution / 2.0f);
		}

	
		
	}

	void Camera::LateUpdate()
	{

	}
	void Camera::Render()
	{
	}
}