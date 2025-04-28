#include "MESkeleton.h"


namespace ME
{

	Skeleton::Skeleton()
		:Resource(enums::eResourceType::Skeleton)
	{

	}
	Skeleton::~Skeleton()
	{

	}

	HRESULT Skeleton::Save(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
	HRESULT Skeleton::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	void Skeleton::BuildSkeleton(aiNode* node)
	{
		std::string nodeName = node->mName.C_Str();

		if (nodeName == "RootNode")
		{
			mRootBoneName = nodeName;
		}

		if (mBoneNameToIndexMap.contains(nodeName))
		{
			int boneIndex = mBoneNameToIndexMap[nodeName];

			if (node->mParent)
			{
				std::string parentName = node->mParent->mName.C_Str();
				if (mBoneNameToIndexMap.contains(parentName))
				{
					int parentIndex = mBoneNameToIndexMap[parentName];
					mBones[boneIndex].mParent = &mBones[parentIndex];
					mBones[parentIndex].mChildren.push_back(&mBones[boneIndex]);
				}
			}
		}


		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			BuildSkeleton(node->mChildren[i]);
		}
	}


	void Skeleton::RegisterBone(aiNode* node)
	{
		std::string nodeName= node->mName.C_Str();

		if (mBoneNameToIndexMap.find(nodeName) == mBoneNameToIndexMap.end())
		{
			int idx = static_cast<int>(mBones.size());
			Bone newBone;
			newBone.mName = nodeName;
			newBone.mLocalTransform = ConvertAIMatrixToMatrix(node->mTransformation);
			mBoneNameToIndexMap[nodeName] = idx;
			mBones.push_back(newBone);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			RegisterBone(node->mChildren[i]);
		}


	}
	void Skeleton::CalculateFinalTransform()
	{
		int rootIdx = GetBoneIndex(mRootBoneName);

		if(rootIdx == -1 )
			assert(false && "Root bone not found in skeleton.");

		updateBoneRecursive(rootIdx, math::Matrix::Identity);
	}

	void Skeleton::updateBoneRecursive(int boneIndex, const math::Matrix& parentTransform)
	{
		Bone& bone = mBones[boneIndex];

		math::Matrix globalMatrix = bone.mLocalTransform * parentTransform;
		bone.FinalTransform = globalMatrix * bone.mOffsetMatrix;

		for (auto child : bone.mChildren)
		{
			updateBoneRecursive(child->mIndex, globalMatrix);
		}
	}


	math::Matrix Skeleton::ConvertAIMatrixToMatrix(aiMatrix4x4& aiMat)
	{
		math::Matrix mat;

		mat._11 = aiMat.a1; mat._12 = aiMat.b1; mat._13 = aiMat.c1; mat._14 = aiMat.d1;
		mat._21 = aiMat.a2; mat._22 = aiMat.b2; mat._23 = aiMat.c2; mat._24 = aiMat.d2;
		mat._31 = aiMat.a3; mat._32 = aiMat.b3; mat._33 = aiMat.c3; mat._34 = aiMat.d3;
		mat._41 = aiMat.a4; mat._42 = aiMat.b4; mat._43 = aiMat.c4; mat._44 = aiMat.d4;

		return mat;
	}
}