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
		:Resource(enums::eResourceType::Animation3D)
		, mAnimator(nullptr)
		,mSkeleton(nullptr)
		,mPrevRootPos(Vector3::Zero)
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



	}

	void Animation3D::Render()
	{
		
	}


	void Animation3D::UpdateAnimation(float currentTime, Skeleton* skeleton, Animator3D* animator)
	{


		if (skeleton == nullptr || animator == nullptr)
			return;

		float timeInTicks = currentTime * mTickersPerSecond;
		std::unordered_set<std::string> updatedBones;
		Vector3 currentRootPos = Vector3::Zero;


		for (auto& bones : boneAnimations) 
		{
		
			int boneindex = skeleton->GetBoneIndexToMatchWithAnim(bones.boneName);

			if (boneindex == -1)
				continue;

			math::Matrix localTransform = InterpolateLocalTransform(bones, timeInTicks, bones.boneName, skeleton);
			skeleton->mBones[boneindex].mLocalTransform = localTransform;

			updatedBones.insert(bones.boneName);

			if (bones.boneName == "mixamorig:Hips" || bones.boneName == "Hips")
				currentRootPos = localTransform.Translation();
		}
		for (auto& bone : skeleton->mBones)
		{
			if (updatedBones.find(bone.mName) == updatedBones.end())
			{
				bone.mLocalTransform = bone.mDefaultLocalTransform;
			}
		}

		Transform* tr = animator->GetOwner()->GetComponent<Transform>();

		if (animator->GetApplyRootMotion())
		{

			Vector3 prevRootPos = animator->GetPrevRootPos();
			Vector3 delta = currentRootPos - prevRootPos;


			delta.y = 0;

			Vector3 currentPos = tr->GetPosition();
			tr->SetPosition(currentPos + delta);

	
			animator->SetPrevRootPos(currentRootPos);
		}


		skeleton->CalculateFinalTransform();
	}

	void Animation3D::CreateAnimation(const std::wstring& name, const std::wstring& path)
	{
		mName = name;
		SetName(name);

		Assimp::Importer importer;
		std::string path_str = std::string(path.begin(), path.end());

		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene* animScene = importer.ReadFile(path_str,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_RemoveRedundantMaterials | //  쓰레기 재질 제거
			//  잘못된 데이터 자동 제거
			//aiProcess_OptimizeMeshes |           //  메쉬 최적화
			aiProcess_ImproveCacheLocality |   //  GPU 캐시 최적화
			//aiProcess_MakeLeftHanded |
			aiProcess_JoinIdenticalVertices |
			//aiProcess_FlipWindingOrder | //  좌표계 변환  
			aiProcessPreset_TargetRealtime_Quality
		);

		if (animScene && animScene->mNumAnimations > 0)
		{
			aiAnimation* anim = animScene->mAnimations[0];
			CreateFromAssimp(anim);
			
			this->SetPath(path);

		}


	}
	void Animation3D::Reset()
	{
	
		mPrevRootPos = Vector3::Zero;
		mRootMotionTotalOffset = Vector3::Zero;
		mRootMotionBasePosition = mAnimator->GetOwner()->GetComponent<Transform>()->GetPosition();
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
			std::string boneName = channel->mNodeName.C_Str();
		
			boneName = extractBoneName(boneName);
			boneAnim.boneName = boneName;

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

	std::string Animation3D::extractBoneName(std::string& name)
	{
		//size_t lastSlash = name.find_last_of("/|:");
		//std::string newName;
		//
		//if (lastSlash != std::string::npos)
		//{
		//	newName = name.substr(lastSlash + 1);
		//}
		//else
		//{
		//	newName = name;
		//}

		size_t assimpFbxPos = name.find("$AssimpFbx$");
		if(assimpFbxPos != std::string::npos)
		{
			name = name.substr(0, assimpFbxPos -1);
		}

		//std::string finalName = "";
		//
		//for (char c : newName)
		//{
		//	finalName += std::tolower(static_cast<unsigned char>(c));
		//}
		//
		return name;
	}


	math::Matrix Animation3D::InterpolateLocalTransform(const BoneAnimation& boneAnim, float currentTime,const std::string& boneName, Skeleton* skeleton)
	{
		Vector3 interpolatedPos = InterpolatePosition(boneAnim.positions, currentTime);

		if (boneName == "Hips" || boneName == "mixamorig:Hips")
		{
			if (!mAnimator->GetApplyRootMotion())
				interpolatedPos = Vector3::Zero;
		}

		Quaternion interpolatedRot = InterpolateRotation(boneAnim.rotations, currentTime);
		int boneIdx = skeleton->GetBoneIndex(boneName);


		//Quaternion finalRot = interpolatedRot;
		//
		//
		Matrix baseTransform = Matrix::Identity;
		
		if (boneIdx != -1)
		{
			 baseTransform = skeleton->mBonesTransform[boneIdx];
		}
		//
		Vector3 interpolatedScale = InterpolateScale(boneAnim.scales, currentTime);

		Matrix transform =
			Matrix::CreateFromQuaternion(interpolatedRot) *
			Matrix::CreateTranslation(interpolatedPos);

		//Matrix::CreateScale(interpolatedScale) *

		transform = transform * baseTransform;

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