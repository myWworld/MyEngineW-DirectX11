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
		void Render()override;

		virtual void OnCollisionEnter(Collider* other);
		virtual void OnCollisionStay(Collider* other);
		virtual void OnCollisionExit(Collider* other);

		Vector2 GetOffset() const{ return mOffset; }
		void SetOffset(Vector2 offset) { mOffset = offset; }

		UINT32 GetID() const{ return mID; }
		Vector2 GetSize() const { return mSize; }

		void SetSize(Vector2 size) { mSize = size; }

		enums::eColliderType GetColliderType() const{ return mType; }

	private:

		static UINT32 mCollisionID;

		UINT32 mID;
		Vector2 mOffset;
		Vector2 mSize;

		enums::eColliderType mType;

	};

}
