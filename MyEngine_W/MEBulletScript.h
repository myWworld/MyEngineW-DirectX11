#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "MEMath.h"
#include "MEGameObject.h"
#include "MEObjectPool.h"


namespace ME
{


	class BulletScript :public Script
	{
	public:

		BulletScript();
		~BulletScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		void SetPool(ObjectPool<GameObject>* pool) { mPool = pool; }

		private:
			void resetBulletData();


	private:

		float mDeadTime;
		float mDeadTimer;

		ObjectPool<GameObject>* mPool;

	};

}
