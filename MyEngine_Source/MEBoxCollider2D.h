#pragma once
#include "MECollider.h"

namespace ME
{


	class BoxCollider2D:public Collider
	{
	public:


		BoxCollider2D();
		~BoxCollider2D();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		 
		 void SetRotation(float rot) { mRot = rot; }
		 Vector3 GetCentralPoint() { return mCentralPoint; }

		 bool IsRotate() { return mbIsRotate; }
		 float GetHeight() { return mHeight; }
		 float GetWidth() { return mWidth; }

		 POINT* GetPoint() { return mPoint; }

	private:
	
		Vector3 RotateCollider(float rot, int x, int y, int width, int height);
	
	private:
		
		float mRot;
		Vector3 mCentralPoint;

		float mWidth, mHeight;

		bool mbIsRotate;
		POINT* mPoint;

	};

}
