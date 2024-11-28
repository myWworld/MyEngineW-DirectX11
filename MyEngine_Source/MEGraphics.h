#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#include "CommonInclude.h"
#include <wrl.h>

namespace ME::graphics
{

	struct Vertex
	{
		math::Vector3 pos;
		math::Vector4 color;
		math::Vector2 uv;
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

	struct GpuBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
		D3D11_BUFFER_DESC desc = {};

		GpuBuffer() = default;
		virtual ~GpuBuffer() = default;

	};

}
