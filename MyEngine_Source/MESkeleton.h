#pragma once

#include "CommonInclude.h"
#include "MEBone.h"
#include "MEResource.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace ME
{
	class Skeleton : public Resource
	{
	public:
		Skeleton();
		~Skeleton();

		std::vector<Bone> mBones;
		std::unordered_map<std::string, int> mBoneNameToIndexMap;
		std::string mRootBoneName;

	public:


		HRESULT Save(const std::wstring& path) override;
		HRESULT Load(const std::wstring& path)override;

		void BuildSkeleton(aiNode* node);

		int GetBoneIndex(const std::string& name ) const
		{
			auto it = mBoneNameToIndexMap.find(name);
			if (it != mBoneNameToIndexMap.end())
			{
				return it->second;
			}
			return -1; 
		}

		void CalculateFinalTransform();
		void RegisterBone(aiNode* node);

	private:
		void updateBoneRecursive(int boneIndex, const math::Matrix& parentTransform);
		
		math::Matrix ConvertAIMatrixToMatrix(aiMatrix4x4& aiMat);
	};
}

