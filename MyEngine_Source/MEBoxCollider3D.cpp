#include "MEBoxCollider3D.h"
#include "METransform.h"
#include "MEGameObject.h"
#include "MERenderer.h"
#include "MECamera.h"


namespace ME
{
	BoxCollider3D::BoxCollider3D()
		:Collider(enums::eColliderType::Box3D)
		, mRot(0)
		, mbIsRotate(false)
	{

	}
	BoxCollider3D::~BoxCollider3D()
	{
	}
	void BoxCollider3D::Initialize()
	{
	}
	void BoxCollider3D::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();

		Vector3 offset = GetOwner()->GetComponent<BoxCollider3D>()->GetOffset();
		float width = (100.0f * GetSize().x);
		float height = (100.0f * GetSize().y);



		if (mRot != 0)
			mCentralPoint = RotateCollider(mRot, pos.x + offset.x, pos.y + offset.y, width, height);
	}
	void BoxCollider3D::LateUpdate()
	{
	}
	void BoxCollider3D::Render()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();



	}

	Vector3 BoxCollider3D::RotateCollider(float rot, int x, int y, int width, int height)//, HDC hdc)
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


		mPoint = points;

		Vector3 centralPoint = Vector3(midx, midy, 0.0f);

		return centralPoint;
	}

	bool BoxCollider3D::IntersectWith(BoxCollider3D* other)
	{
		Transform* leftTr = this->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = other->GetOwner()->GetComponent<Transform>();

		Vector3 leftPos = leftTr->GetPosition() + this->GetOffset();
		Vector3 rightPos = rightTr->GetPosition() + other->GetOffset();

		Vector3 leftSize = this->GetSize(); //* 100.0f;
		Vector3 rightSize = other->GetSize(); //* 100.0f;

		BoxCollider3D* leftBC3D = this;
		BoxCollider3D* rightBC3D = other;

		enums::eColliderType leftType = this->GetColliderType();
		enums::eColliderType rightType = other->GetColliderType();

		if (leftType == enums::eColliderType::Box3D
			&& rightType == enums::eColliderType::Box3D)
		{
			Vector3 LHeight = Vector3(0, leftSize.y, 0);
			Vector3 RHeight = Vector3(0, rightSize.y, 0);

			Vector3 leftCenterPos = leftPos + (LHeight / 2.0f);
			Vector3 rightCenterPos = rightPos + (RHeight / 2.0f);


			if (leftBC3D->IsRotate())
			{

				leftCenterPos = leftBC3D->GetCentralPoint();

				leftSize.y = leftBC3D->GetHeight();
				leftSize.x = leftBC3D->GetWidth();

			}

			if (rightBC3D->IsRotate())
			{
				rightCenterPos = rightBC3D->GetCentralPoint();

				rightSize.y = rightBC3D->GetHeight();
				rightSize.x = rightBC3D->GetWidth();

			}



			//AABB 충돌 rect-rect
			if (fabs(leftCenterPos.x - rightCenterPos.x) < fabs(leftSize.x / 2.0f + rightSize.x / 2.0f)
				&& fabs(leftCenterPos.y - rightCenterPos.y) < fabs(leftSize.y / 2.0f + rightSize.y / 2.0f)
				&& fabs(leftCenterPos.z - rightCenterPos.z) < fabs(leftSize.z / 2.0f + rightSize.z / 2.0f))
			{
				return true;
			}
	}
}