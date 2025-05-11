#pragma once
#include "MEComponent.h"

namespace ME
{
	using namespace ME::math;

	class Camera :public Component
	{
	public:
	
		enum class eProjectionType
		{
			Perspective,
			Orthographic,
		};

		static Matrix GetGpuViewMatrix() { return ViewMatrix; }
		static Matrix GetGpuProjectionMatrix() { return ProjectionMatrix; }
		static void SetGpuViewMatrix(Matrix matrix) { ViewMatrix = matrix; }
		static void SetGpuProjectionMatrix(Matrix matrix) { ProjectionMatrix = matrix; }

		Camera();
		~Camera();


		void Initialize() override;
		void Update()override;
		void LateUpdate()override;
		void Render()override;

		void CreateViewMatrix();
		void CreateProjectionMatrix(eProjectionType type);

		void SetProjectionMatrix(eProjectionType type) { mProjectionType = type; }
		void SetSize(float size) { mSize = size; }

		void SetTarget(GameObject* obj) { mtarget = obj; }

		

	private:

		//std::vector<GameObject*> mGameObjects;

		static Matrix ViewMatrix;
		static Matrix ProjectionMatrix;

		eProjectionType mProjectionType;

		Matrix mViewMatrix;
		Matrix mProjectionMatrix;
		
		float mAspectRatio;
		float mNear;
		float mFar;
		float mSize;

		Vector3 mTargetPos;
		GameObject* mtarget;
	
	};

}
