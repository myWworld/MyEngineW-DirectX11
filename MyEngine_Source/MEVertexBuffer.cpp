#include "MEVertexBuffer.h"


namespace ME::graphics
{


	ME::graphics::VertexBuffer::VertexBuffer()
	{	
	}

	ME::graphics::VertexBuffer::~VertexBuffer()
	{
	}

	bool ME::graphics::VertexBuffer::Create(const std::vector<Vertex>& vertexes)
	{
		desc.ByteWidth = sizeof(Vertex) * vertexes.size();
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sub = {};
		sub.pSysMem = vertexes.data();

		if (!(GetDevice()->CreateBuffer(&desc, &sub, buffer.GetAddressOf())))
			assert(NULL, "Create Vertex buffer failed!");
		return true;
	
	}

	void ME::graphics::VertexBuffer::Bind() 
	{
		UINT offset = 0;
		UINT vertexSize = sizeof(Vertex);
		GetDevice()->BindVertexBuffer(0, 1, buffer.GetAddressOf(), &vertexSize, &offset);
	}

}

