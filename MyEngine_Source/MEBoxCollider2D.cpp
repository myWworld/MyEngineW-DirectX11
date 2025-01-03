#include "MEBoxCollider2D.h"
#include "METransform.h"
#include "MEGameObject.h"
#include "MERenderer.h"
#include "MECamera.h"


namespace ME
{
	BoxCollider2D::BoxCollider2D()
		:Collider(enums::eColliderType::Rect2D)
		,mRot(0)
		,mbIsRotate(false)
	{

	}
	BoxCollider2D::~BoxCollider2D()
	{
	}
	void BoxCollider2D::Initialize()
	{
	}
	void BoxCollider2D::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		Vector3 offset = GetOwner()->GetComponent<BoxCollider2D>()->GetOffset();
		float width = (100.0f * GetSize().x);
		float height = (100.0f * GetSize().y);



		if (mRot != 0)
			mCentralPoint = RotateCollider(mRot, pos.x + offset.x, pos.y + offset.y, width, height);
	}
	void BoxCollider2D::LateUpdate()
	{
	}
	void BoxCollider2D::Render()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		

		
	}

	Vector3 BoxCollider2D::RotateCollider(float rot, int x ,int y, int width,int height)//, HDC hdc)
	{
		
		float radian = rot * (3.14159265f / 180.0f);

		POINT points[4];
	
		points[0].x = x;
		points[0].y = y;

		// 오른쪽 위 꼭짓점
		points[1].x = x + (int)(width * cos(radian));
		points[1].y = y + (int)(width * sin(radian));

		// 오른쪽 아래 꼭짓점
		points[2].x = x + (int)(width * cos(radian) - height * sin(radian));
		points[2].y = y + (int)(width * sin(radian) + height * cos(radian));

		// 왼쪽 아래 꼭짓점
		points[3].x = x - (int)(height * sin(radian));
		points[3].y = y + (int)(height * cos(radian));

		// 회전된 사각형 그리기
		//Polygon(hdc, points, 4);


		float midx = (points[0].x + points[1].x + points[2].x + points[3].x) / 4;
		float midy = (points[0].y + points[1].y + points[2].y + points[3].y) / 4;

		mWidth = max(fabs(points[1].x - points[2].x), fabs(points[0].x - points[3].x));

		mHeight = max(fabs(points[1].y - points[2].y), fabs(points[0].y - points[3].y));

	
		mPoint =  points;

		Vector3 centralPoint = Vector3(midx, midy, 0.0f);

		return centralPoint;
	}
}