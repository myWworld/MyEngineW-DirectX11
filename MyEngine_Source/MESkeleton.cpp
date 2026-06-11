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
		size_t nodeHash = 0;

		if (nodeName == "RootNode")
		{
			nodeHash = std::hash<std::string>{}(nodeName);
			mRootBoneHash = nodeHash;
		}
		else
		{
			nodeName = ExtractBoneName(nodeName);
			nodeHash = std::hash<std::string>{}(nodeName);
		}

		if (mBoneNameToIndexMap.contains(nodeHash))
		{
			int boneIndex = mBoneNameToIndexMap[nodeHash];

			if (node->mParent)
			{
				std::string parentName = node->mParent->mName.C_Str();
				size_t parentHash = 0;

				if (parentName == "RootNode")
				{
					parentHash = std::hash<std::string>{}(parentName);
				}
				else
				{
					parentName = ExtractBoneName(parentName);
					parentHash = std::hash<std::string>{}(parentName);
				}

				if (mBoneNameToIndexMap.contains(parentHash))
				{
					int parentIndex = mBoneNameToIndexMap[parentHash];

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
		std::string nodeName = node->mName.C_Str();
		size_t nodeHash = 0;

		bool bSkiped = false;

		if (nodeName == "RootNode")
		{
			nodeHash = std::hash<std::string>{}(nodeName);
		}
		else
		{
			std::string pureName = ExtractBoneName(nodeName); // 네임스페이스 제거
			nodeHash = std::hash<std::string>{}(pureName);    // 순수 이름 해싱
		}

		if (mBoneNameToIndexMap.find(nodeHash) == mBoneNameToIndexMap.end() && bSkiped == false)
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

			mBoneNameToIndexMap[nodeHash] = idx;
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
				
				boneName = ExtractBoneName(boneName);
				size_t boneHash = std::hash<std::string>{}(boneName);

				auto iter = mBoneNameToIndexMap.find(boneHash);

	
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
		size_t nameHash = std::hash<std::string>{}(name);

		auto it = mBoneNameToIndexMap.find(nameHash);


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
				size_t newNameHash = std::hash<std::string>{}(newName);

				auto it2 = mBoneNameToIndexMap.find(newNameHash);

				if (it2 != mBoneNameToIndexMap.end())
				{
					return it2->second;
				}
			}
		}
		return -1;
	}

	int Skeleton::GetBoneIndex(const size_t& hash) const
	{
		auto it = mBoneNameToIndexMap.find(hash);


		if (it != mBoneNameToIndexMap.end())
		{
			return it->second;
		}

			return -1;

	
	}

	int Skeleton::GetBoneIndex(const std::string& name) const
	{
		size_t nameHash = std::hash<std::string>{}(name);
		int idx = GetBoneIndex(nameHash);

		if (idx != -1)
		{
			return idx;
		}
		else
		{
			auto iter = BoneNameManualMapping.find(name);

			if (iter != BoneNameManualMapping.end())
			{
				std::string newName = iter->second;
				size_t newNameHash = std::hash<std::string>{}(newName);

				auto it2 = mBoneNameToIndexMap.find(newNameHash);

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
		static const size_t leftHandHash = std::hash<std::string>{}("LeftHand");
		auto it = mBoneNameToIndexMap.find(leftHandHash);

		if (it == mBoneNameToIndexMap.end())
			return nullptr;

		return &mBones[it->second];
	
	}

	Bone* Skeleton::GetRightHandTransform()
	{
		static const size_t rightHandHash = std::hash<std::string>{}("RightHand");
		auto it = mBoneNameToIndexMap.find(rightHandHash);

		if (it == mBoneNameToIndexMap.end())
			return nullptr;

		return &mBones[it->second];
	}


	void Skeleton::CalculateFinalTransform()
	{
		int rootIdx = GetBoneIndex(mRootBoneHash);

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

		bone.mWolrdTransform = globalMatrix;

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