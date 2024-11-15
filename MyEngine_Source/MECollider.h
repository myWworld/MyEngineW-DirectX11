#pragma once
#include "MEComponent.h"


namespace ME
{
	class Collider:public Component
	{

	public:
		
		Collider(enums::eColliderType type);
		~Collider();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render(HDC hdc)override;

		virtual void OnCollisionEnter(Collider* other);
		virtual void OnCollisionStay(Collider* other);
		virtual void OnCollisionExit(Collider* other);

		Vector2 GetOffset() { return mOffset; }
		void SetOffset(Vector2 offset) { mOffset = offset; }

		UINT32 GetID() { return mID; }
		Vector2 GetSize() { return mSize; }

		void SetSize(Vector2 size) { mSize = size; }

		enums::eColliderType GetColliderType() { return mType; }

	private:

		static UINT32 mCollisionID;

		UINT32 mID;
		Vector2 mOffset;
		Vector2 mSize;

		enums::eColliderType mType;

	};

}
