#include "METransform.h"

namespace ME
{

	ME::Transform::Transform()
		:Component(enums::eComponentType::Trasnform)
		, mScale(Vector2::One)
		, mRotation(0.0f)
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
	}

	void ME::Transform::Render( )
	{
	}

}