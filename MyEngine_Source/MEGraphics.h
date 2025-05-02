#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "CommonInclude.h"
#include <wrl.h>

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#define CBSLOT_TRANSFORM 0
#define CBSLOT_ANIMATION 1

namespace ME::graphics
{

	struct Vertex
	{
		math::Vector3 pos;
		math::Vector4 color;
		math::Vector3 normal;
		int boneIndices[4] = { 0 };
		float boneWeight[4] = { 0.0f };
		math::Vector2 uv;

		void AddBoneData(int boneIndex, float weight)
		{
			for (int i = 0; i < 4; i++)
			{
				if (boneWeight[i] == 0.0f)
				{
					boneIndices[i] = boneIndex;
					boneWeight[i] = weight;
					return;
				}
			}
		}

		void NormalizeBoneWeights()
		{
			float totalWeight = 0.0f;
			for (int i = 0; i < 4; ++i)
				totalWeight += boneWeight[i];

			if (totalWeight > 0.0f)
			{
				for (int i = 0; i < 4; ++i)
					boneWeight[i] /= totalWeight;
			}
			else
			{
				boneIndices[0] = 0;
				boneWeight[0] = 1.0f;
			}
		}
	};


	enum class eShaderStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS,
		All,
		End,
	};

	enum class eCBType
	{
		Transform,
		Animation,
		None,
		End,
	};

	enum class eSamplerType
	{
		
		Point,
		Linear,
		Anisotropic,
		PostProcess,
		End,
	};

	enum class eRenderingMode
	{
		Opaque,
		CutOut,
		Transparent,
		PostProcess,
		End,
	};

	enum class eTextureType
	{
		Albedo,
		Normal,
		Specular,
		Smoothness,
		Metalic,
		Sprite,
		End,
	};

	enum class eRasterizerState
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireFrame,
		End,
	};

	enum class eBlendState
	{
		AlphaBlend,
		OneOne,
		End,
	};

	enum class eDepthStencilState
	{
		DepthNone,
		LessEqual,
		End,
	};

	struct GpuBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
		D3D11_BUFFER_DESC desc = {};

		GpuBuffer() = default;
		virtual ~GpuBuffer() = default;

	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		math::Matrix world;
		math::Matrix view;
		math::Matrix projection;

	};

	CBUFFER(AnimationCB, CBSLOT_ANIMATION)
	{
		math::Matrix BoneMatrices[256];

	};

}
