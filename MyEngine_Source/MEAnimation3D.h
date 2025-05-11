#pragma once

#include "METexture.h"
#include "MEResource.h"
#include "CommonInclude.h"


#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace ME
{
	using namespace math;

	class Animation3D :public Resource
	{
	public:

		struct VectorKey
		{
			float time;
			Vector3 value;
		};

		struct QuatKey
		{
			float time;
			Quaternion value;
		};

		struct BoneAnimation
		{
			std::string boneName;
			std::vector<VectorKey> positions;
			std::vector<QuatKey> rotations;
			std::vector<VectorKey> scales;
		};

		Animation3D();
		~Animation3D();

		HRESULT Save(const std::wstring& path) override;
		HRESULT Load(const std::wstring& path) override;

		void Update();
		void Render();

		void CreateAnimation(const std::wstring& name, const std::wstring& path);

		void Reset();

		bool IsComplete() const { return mbComplete; }
		void SetAnimator(class Animator3D* animator) { mAnimator = animator; }


	public:

		Vector3 mPrevRootPos = Vector3::Zero;
		Vector3 mRootMotionTotalOffset = Vector3::Zero;
		Vector3 mRootMotionBasePosition = Vector3::Zero;

		std::wstring mName;
		float mDuration;
		float mTickersPerSecond;
		std::vector<BoneAnimation> boneAnimations;


	private:

		void CreateFromAssimp(aiAnimation* anim);

		std::string extractBoneName(std::string& name);
		
		math::Matrix InterpolateLocalTransform(const BoneAnimation& boneAnim, float currentTime, std::string boneName);
		math::Vector3 InterpolatePosition(const	std::vector<VectorKey>& pos, float currentTime);
		math::Quaternion InterpolateRotation(const std::vector<QuatKey>& rotation, float currentTime);
		math::Vector3 InterpolateScale(const std::vector<VectorKey>& scale, float currentTime);

	
	private:

		class Animator3D* mAnimator;
		class Skeleton* mSkeleton;
	

		float mTime;
		bool mbComplete;


	};

}
