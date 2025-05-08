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
		std::vector<Matrix> mBonesTransform;
		std::string mRootBoneName;

		enums::eModelType mModelType;

		std::unordered_set<int> visited;

	public:


		HRESULT Save(const std::wstring& path) override;
		HRESULT Load(const std::wstring& path)override;

		void BuildSkeleton(aiNode* node);

		int GetBoneIndex(const std::string& name) const;
		int GetBoneIndexToMatchWithAnim(const std::string& name) const;

		void CalculateFinalTransform();
		void RegisterBone(aiNode* node, std::unordered_map<std::string, math::Matrix>& preRot);
		void RegisterSkinData(const aiScene* scene);

		void SetModelType( enums::eModelType type) { mModelType = type; }
		enums::eModelType GetModelType() { return mModelType; }

		std::string ExtractBoneName(const std::string& name) const;

	private:
		void updateBoneRecursive(int boneIndex, const math::Matrix& parentTransform);
		
		
		math::Matrix ConvertAIMatrixToMatrix(aiMatrix4x4& aiMat);
	};
}

