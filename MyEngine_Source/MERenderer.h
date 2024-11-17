#pragma once
#include "MECamera.h"
#include "MEGraphicDevice_DX11.h"

#include "MEVertexBuffer.h"

namespace ME::renderer 
{
	extern Camera* mainCamera;
	extern Camera* playerCamera;

	extern std::vector<graphics::Vertex> vertexes;
	extern std::vector<UINT> indices;

	extern graphics::VertexBuffer vertexBuffer;
	extern ID3D11Buffer* indexBuffer;
	extern ID3D11Buffer* constantBuffer;


	extern ID3D11InputLayout* inputLayouts;

	void Initialize();
	void Release();

}

