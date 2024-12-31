#pragma once
#include "MECamera.h"
#include "MEGraphicDevice_DX11.h"

#include "MEConstantBuffer.h"



using namespace ME::math;
using namespace ME::graphics;

namespace ME::renderer 
{
	extern Camera* mainCamera;
	extern Camera* playerCamera;

	

	extern graphics::ConstantBuffer constantBuffers[(UINT)graphics::eCBType::End];
	
	extern Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	void Initialize();
	void Release();

}

