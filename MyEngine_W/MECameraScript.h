#pragma once
#include "../MyEngine_Source/MEScript.h"
#include "MEMath.h"

namespace ME
{


	class CameraScript:public Script
	{
	public:


		CameraScript();
		~CameraScript();

		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;



	private:

		Vector2 mPrevMousePos;
		Vector2 mCurMousePos;
		float mMouseSpeed;
	};

}
