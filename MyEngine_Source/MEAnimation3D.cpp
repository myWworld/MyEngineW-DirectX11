#include "MEAnimation3D.h"
#include "METime.h"

#include "METransform.h"
#include "MEAnimator3D.h"
#include "MEGameObject.h"
#include "MERenderer.h"
#include "MECamera.h"
#include "MESceneManager.h"
#include "MESkeleton.h"

namespace ME
{
	Animation3D::Animation3D()
		:Resource(enums::eResourceType::Animation)
		, mAnimator(nullptr)
		, mbComplete(false)
	{
	}

	Animation3D::~Animation3D()
	{
	}
	HRESULT Animation3D::Save(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	HRESULT Animation3D::Load(const std::wstring& path)
	{
	
		return E_NOTIMPL;
	}
	void Animation3D::Update()
	{

		if (mSkeleton == nullptr)
		{
			if(mAnimator != nullptr)
				mSkeleton = mAnimator->GetSkeleton();
		}

		if (mbComplete)
			return;

		mTime += Time::DeltaTime();

		if (mTime > mDuration)
		{
			mbComplete = true;
			mTime = mDuration;
			return;
		}

		for (auto bones : boneAnimations)
		{
			int boneindex = mSkeleton->GetBoneIndex(bones.boneName);

			if (boneindex == -1)
				continue;

			math::Matrix localTransform = InterpolateLocalTransform(bones, mTime);
			mSkeleton->mBones[boneindex].mLocalTransform = localTransform;

		}

		mSkeleton->CalculateFinalTransform();


	
	}

	void Animation3D::Render()
	{
		
	}

	void Animation3D::CreateAnimation(const std::wstring& name, const std::wstring& path)
	{
		mName = name;
		SetName(name);

		Assimp::Importer importer;
		std::string path_str = std::string(path.begin(), path.end());

		const aiScene* animScene = importer.ReadFile(path_str,
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_LimitBoneWeights
		);


		if (animScene && animScene->mNumAnimations > 0)
		{
			aiAnimation* anim = animScene->mAnimations[0];
			CreateFromAssimp(anim);
			mSkeleton = mAnimator->GetSkeleton();
			this->SetPath(path);

		}


	}
	void Animation3D::Reset()
	{
		mTime = 0;
		mbComplete = false;
	}

	void Animation3D::CreateFromAssimp(aiAnimation* anim)
	{
	
		std::string name = anim->mName.C_Str();

		this->mName = std::wstring(name.begin(), name.end());
		this->mDuration = static_cast<float>(anim->mDuration);
		this->mTickersPerSecond = static_cast<float>(anim->mTicksPerSecond != 0 ? anim->mTicksPerSecond : 25.0f);


		for (unsigned int i = 0; i < anim->mNumChannels; i++)
		{
			aiNodeAnim* channel = anim->mChannels[i];

			BoneAnimation boneAnim;
			boneAnim.boneName = channel->mNodeName.C_Str();

			for (unsigned int pos = 0; pos < channel->mNumPositionKeys; pos++)
			{
				aiVectorKey aiPosKey = channel->mPositionKeys[pos];

				VectorKey key;
				key.time = static_cast<float>(aiPosKey.mTime);
				key.value = Vector3(aiPosKey.mValue.x, aiPosKey.mValue.y, aiPosKey.mValue.z);
				boneAnim.positions.push_back(key);
			}

			for (unsigned int pos = 0; pos < channel->mNumRotationKeys; pos++)
			{
				aiQuatKey aiQuatKey = channel->mRotationKeys[pos];

				QuatKey key;
				key.time = static_cast<float>(aiQuatKey.mTime);
				key.value = Quaternion(aiQuatKey.mValue.x, aiQuatKey.mValue.y, aiQuatKey.mValue.z, aiQuatKey.mValue.w);
				boneAnim.rotations.push_back(key);
			}

			for (unsigned int pos = 0; pos < channel->mNumScalingKeys; pos++)
			{
				aiVectorKey aiPosKey = channel->mScalingKeys[pos];

				VectorKey key;
				key.time = static_cast<float>(aiPosKey.mTime);
				key.value = Vector3(aiPosKey.mValue.x, aiPosKey.mValue.y, aiPosKey.mValue.z);
				boneAnim.scales.push_back(key);
			}

			this->boneAnimations.push_back(boneAnim);
		}

	}


	math::Matrix Animation3D::InterpolateLocalTransform(const BoneAnimation& boneAnim, float currentTime)
	{
		Vector3 interpolatedPos = InterpolatePosition(boneAnim.positions, currentTime);
		Quaternion interpolatedRot = InterpolateRotation(boneAnim.rotations, currentTime);
		Vector3 interpolatedScale = InterpolateScale(boneAnim.scales, currentTime);

		Matrix transform =
			Matrix::CreateScale(interpolatedScale) *
			Matrix::CreateFromQuaternion(interpolatedRot) *
			Matrix::CreateTranslation(interpolatedPos);

		return transform;
	}


	math::Vector3 Animation3D::InterpolatePosition(const	std::vector<VectorKey>& pos, float currentTime)
	{
		if (pos.size() == 1)
			return pos[0].value;

		for (size_t i = 0; i < pos.size() - 1; i++)
		{
			if (currentTime < pos[i + 1].time)
			{
				float t = (currentTime - pos[i].time) / (pos[i + 1].time - pos[i].time);
				
				return Vector3::Lerp(pos[i].value, pos[i + 1].value, t);
			}
		}

		return pos.back().value;
	}
	math::Quaternion Animation3D::InterpolateRotation(const std::vector<QuatKey>& rotation, float currentTime)
	{
		if (rotation.size() == 1)
			return rotation[0].value;

		for (size_t i = 0; i < rotation.size() - 1; i++)
		{
			if (currentTime < rotation[i + 1].time)
			{
				float t = (currentTime - rotation[i].time) / (rotation[i + 1].time - rotation[i].time);

				return Quaternion::Slerp(rotation[i].value, rotation[i + 1].value, t);
			}
		}

		return rotation.back().value;
	}
	math::Vector3 Animation3D::InterpolateScale(const std::vector<VectorKey>& scale, float currentTime)
	{
		if (scale.size() == 1)
			return scale[0].value;

		for (size_t i = 0; i < scale.size() - 1; i++)
		{
			if (currentTime < scale[i + 1].time)
			{
				float t = (currentTime - scale[i].time) / (scale[i + 1].time - scale[i].time);

				return Vector3::Lerp(scale[i].value, scale[i + 1].value, t);
			}
		}

		return scale.back().value;
	}
}