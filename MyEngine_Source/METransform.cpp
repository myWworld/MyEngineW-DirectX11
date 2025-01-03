#include "METransform.h"
#include "MEGraphics.h"
#include "MECamera.h"
#include "MEConstantBuffer.h"
#include "MERenderer.h"

namespace ME
{

	ME::Transform::Transform()
		:Component(enums::eComponentType::Trasnform)
		,mWorldMatrix(Matrix::Identity)
		,mScale(Vector3::One)
		,mPosition(Vector3::Zero)
		,mRotation(Vector3::Zero)
	{
	}

	ME::Transform::~Transform()
	{
	}

	void ME::Transform::Initialize()
	{

	}

	void ME::Transform::Update()
	{
	}

	void ME::Transform::LateUpdate()
	{
		Matrix scale = Matrix::CreateScale(mScale.x, mScale.y, mScale.z);

		Matrix rotation = Matrix::CreateRotationX(math::Radian(mRotation.x));
		rotation *= Matrix::CreateRotationY(math::Radian(mRotation.y));
		rotation *= Matrix::CreateRotationZ(math::Radian(mRotation.z));

		Matrix translation = Matrix::CreateTranslation(mPosition);

		mWorldMatrix = scale * rotation * translation;

		mForward = Vector3::TransformNormal(Vector3::Forward, rotation);
		mRight = Vector3::TransformNormal(Vector3::Right, rotation);
		mUp= Vector3::TransformNormal(Vector3::Up, rotation);


	}

	void ME::Transform::Render( )
	{
	}

	void Transform::Bind()
	{
		graphics::TransformCB cbData = {};
		cbData.world = GetWorldMatrix();
		cbData.view = Camera::GetGpuViewMatrix();
		cbData.projection = Camera::GetGpuProjectionMatrix();

		graphics::ConstantBuffer* cb = renderer::constantBuffers[CBSLOT_TRANSFORM];

		cb->SetData(&cbData);
		cb->Bind(eShaderStage::All);
	}

}