#pragma once

#include "MEGraphicDevice_DX11.h"

namespace ME::graphics
{

	class ConstantBuffer :public GpuBuffer
	{

	public:

		ConstantBuffer();
		~ConstantBuffer();

		bool Create(eCBType type, UINT size, void* data = nullptr);

		void SetData(void* data) const;
		void Bind(eShaderStage stage) const;

	private:
		UINT mSize;
		eCBType mType;

	};



}