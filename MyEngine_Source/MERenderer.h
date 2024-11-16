#pragma once
#include "MECamera.h"
#include "MEGraphicDevice_DX11.h"

namespace ME::renderer 
{
	extern Camera* mainCamera;
	extern Camera* playerCamera;

	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

	extern Vertex vertexes[3];
	extern std::vector<UINT> indices;

	extern ID3D11Buffer* vertexBuffer;
	extern ID3D11Buffer* indexBuffer;
	extern ID3D11Buffer* constantBuffer;


	extern ID3D11InputLayout* inputLayouts;

	void Initialize();
	void Release();

}

