#pragma once
#include "CommonInclude.h"
#include "MEEntity.h"
#include "MEComponent.h"

namespace ME
{	
	using namespace math;

	class Transform: public Component
	{
	public:

		Transform();
		~Transform();
		
		void Initialize() override;
		void Update()  override;
		void LateUpdate()  override;
		void Render()  override;
		
		void SetPosition(Vector2 pos) { mPosition = pos; }
		void SetScale(Vector2 scale) { mScale = scale; }
		void SetRotation(float rotation) { mRotation = rotation; }

		Vector2 GetPosition() const{ return mPosition; }

		Vector2 GetScale() const{ return mScale; }
		float GetRotation() const{ return mRotation; }

	private:
		Vector2 mPosition;
		Vector2 mScale;
		
		float mRotation;

	};

}
