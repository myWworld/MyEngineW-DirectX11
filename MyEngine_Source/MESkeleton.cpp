#include "MESkeleton.h"
#include "MEAnimator3D.h"

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
		//else
		//{
		//	nodeName = ExtractBoneName(nodeName);
		//}

		if (mBoneNameToIndexMap.contains(nodeName))
		{
			int boneIndex = mBoneNameToIndexMap[nodeName];

			if (node->mParent)
			{
				std::string parentName = node->mParent->mName.C_Str();

				//if (parentName != "RootNode")
				//	parentName = ExtractBoneName(parentName);

				if (mBoneNameToIndexMap.contains(parentName))
				{
					int parentIndex = mBoneNameToIndexMap[parentName];

					auto& children = mBones[parentIndex].mChildren;
					if (std::find(children.begin(), children.end(), &mBones[boneIndex]) == children.end())
					{
						mBones[boneIndex].mParent = &mBones[parentIndex];
						children.push_back(&mBones[boneIndex]);
					}
				}
			}
		}


		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			BuildSkeleton(node->mChildren[i]);
		}
	}


	

	void Skeleton::RegisterBone(aiNode* node, std::unordered_map<std::string, math::Matrix>& preRot)
	{
		std::string nodeName= node->mName.C_Str();
		bool bSkiped = false;

		//if(nodeName != "RootNode")
		//	nodeName = ExtractBoneName(nodeName);

		if (mBoneNameToIndexMap.find(nodeName) == mBoneNameToIndexMap.end() && bSkiped == false)
		{
			int idx = static_cast<int>(mBones.size());
			Bone newBone;
			newBone.mName = nodeName;
		
			auto it = preRot.find(nodeName);
			Matrix pureRotation = Matrix::Identity;
			if (it != preRot.end())
				pureRotation = it->second;

			mBonesTransform.push_back(pureRotation);

			Matrix local = ConvertAIMatrixToMatrix(node->mTransformation);

			it = preRot.find(nodeName);
			if (it != preRot.end())
			{
				local = local * it->second;
			}

			newBone.mLocalTransform = local;
			newBone.mDefaultLocalTransform = local;

			mBoneNameToIndexMap[nodeName] = idx;
			newBone.mIndex = idx;
			mBones.push_back(newBone);
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			RegisterBone(node->mChildren[i],preRot);
		}



	}

	void Skeleton::RegisterSkinData(const aiScene* scene)
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];

			for (unsigned int j = 0; j < mesh->mNumBones; j++)
			{
				aiBone* bone = mesh->mBones[j];
				std::string boneName = bone->mName.C_Str();
				//boneName = ExtractBoneName(boneName);

				auto iter = mBoneNameToIndexMap.find(boneName);

	
				if (iter != mBoneNameToIndexMap.end())
				{
					int boneIndex = iter->second;				
					mBones[boneIndex].mOffsetMatrix = ConvertAIMatrixToMatrix(bone->mOffsetMatrix);
				}
				else
				{
					std::cerr << "Bone not found in skeleton(SkinData): " << boneName << std::endl;
				}
			}
		}
	}

	int Skeleton::GetBoneIndexToMatchWithAnim(const std::string& name) const
	{
	

		auto it = mBoneNameToIndexMap.find(name);


		if (it != mBoneNameToIndexMap.end())
		{
			return it->second;
		}
		else
		{
			auto iter = BoneNameManualMapping.find(name);

			if (iter != BoneNameManualMapping.end())
			{
				std::string newName = iter->second;
				auto it2 = mBoneNameToIndexMap.find(newName);

				if (it2 != mBoneNameToIndexMap.end())
				{
					return it2->second;
				}
			}
		}
		return -1;
	}

	int Skeleton::GetBoneIndex(const std::string& name) const
	{
		//auto iter = BoneNameManualMapping.find(name);
		//
		//std::string newName = name;
		//
		//if (iter != BoneNameManualMapping.end())
		//{
		//	newName = iter->second;
		//}

	//	std::string modifiedName = ExtractBoneName(name);

		auto it = mBoneNameToIndexMap.find(name);


		if (it != mBoneNameToIndexMap.end())
		{
			return it->second;
		}
		else
		{
			auto iter = BoneNameManualMapping.find(name);
		
			if (iter != BoneNameManualMapping.end())
			{
				std::string newName = iter->second;
				auto it2 = mBoneNameToIndexMap.find(newName);

				if (it2 != mBoneNameToIndexMap.end())
				{
					return it2->second;
				}
			}
		}
		return -1;
	}

	Bone* Skeleton::GetLeftHandTransform()
	{
		auto it = mBoneNameToIndexMap.find("mixamorig:LeftHand");
		int idx = it->second;

		return  &mBones[idx];
	
	}

	Bone* Skeleton::GetRightHandTransform()
	{
		auto it = mBoneNameToIndexMap.find("mixamorig:RightHand");
		int idx = it->second;

		return &mBones[idx];
	}


	void Skeleton::CalculateFinalTransform()
	{
		int rootIdx = GetBoneIndex(mRootBoneName);

		if (rootIdx == -1)
		{
			return;
			//assert(false && "Root bone not found in skeleton.");
		}


		updateBoneRecursive(rootIdx, math::Matrix::Identity);
	}

	void Skeleton::updateBoneRecursive(int boneIndex, const math::Matrix& parentTransform)
	{
		
		Bone& bone = mBones[boneIndex];

		math::Matrix globalMatrix;

		globalMatrix = bone.mLocalTransform * parentTransform;

		bone.FinalTransform = bone.mOffsetMatrix * globalMatrix;

		for (auto child : bone.mChildren)
		{
			updateBoneRecursive(child->mIndex, globalMatrix); 
		}
	}

	std::string Skeleton::ExtractBoneName(const std::string& name) const
	{
		std::string newName = name;

		size_t pos = newName.find_last_of("/|:");


		if (pos != std::string::npos)
		{
			newName = newName.substr(pos + 1);
		}

		size_t assimpFbxPos = newName.find("$AssimpFbx$");

		if (assimpFbxPos != std::string::npos)
		{
			newName = newName.substr(0, assimpFbxPos - 1);
		}

		//transform(newName.begin(), newName.end(), newName.begin(),
		//	[](unsigned char c) {
		//		return std::tolower(static_cast<unsigned char>(c));
		//	});
	   
		return newName;
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