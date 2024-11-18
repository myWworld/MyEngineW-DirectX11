#pragma once
#include "MECamera.h"
#include "MEGraphicDevice_DX11.h"

#include "MEConstantBuffer.h"

#include "MEMesh.h"

using namespace ME::math;
using namespace ME::graphics;

namespace ME::renderer 
{
	extern Camera* mainCamera;
	extern Camera* playerCamera;

	extern std::vector<graphics::Vertex> vertexes;
	extern std::vector<UINT> indices;

	extern graphics::ConstantBuffer constantBuffers[(UINT)graphics::eCBType::End];
	
	extern Mesh* mesh;

	extern ID3D11Buffer* constantBuffer;
	extern ID3D11InputLayout* inputLayouts;

	void Initialize();
	void Release();

}

