#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "MEMath.h"


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



	private:

		float mDeadTime;
		float mDeadTimer;

	};

}
