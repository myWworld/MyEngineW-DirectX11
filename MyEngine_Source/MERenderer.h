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
	extern ID3D11Buffer* vertexBuffer;
	extern ID3DBlob* vsBlob;
	extern ID3D11VertexShader* vsShader;
	extern ID3DBlob* psBlob;
	extern ID3D11PixelShader* psShader;
	extern ID3D11InputLayout* inputLayouts;

	void Initialize();
	void Release();

}

