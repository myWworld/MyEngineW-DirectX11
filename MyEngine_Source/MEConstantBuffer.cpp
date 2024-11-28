#include "MEConstantBuffer.h"


namespace ME::graphics
{
	ConstantBuffer::ConstantBuffer()
		:mSize(0)
		,mType(eCBType::None)
	{
	}
	ConstantBuffer::~ConstantBuffer()
	{
	}
	bool ConstantBuffer::Create(eCBType type, UINT size, void* data)
	{
		mType = type;
		mSize = size;

		desc.ByteWidth = size;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = data;

		bool success = false;

		if (data == nullptr)
		{
			success = GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
		}
		else
		{
			success = GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf());
		}

		if (!success)
			assert(NULL/*, "Create Constant Buffer failed"*/);

		return true;
	}
	void ConstantBuffer::SetData(void* data) const
	{
		GetDevice()->SetDataGpuBuffer(buffer.Get(), data, mSize);
	}
	void ConstantBuffer::Bind(eShaderStage stage) const
	{
		GetDevice()->BindConstantBuffer(stage, mType, buffer.Get());
	}
}