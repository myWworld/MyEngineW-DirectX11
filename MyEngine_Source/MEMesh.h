#pragma once

#include "MEResource.h"
#include "MEVertexBuffer.h"
#include "MEIndexBuffer.h"


namespace ME
{
	class Mesh: public Resource
	{
	public:


		struct Data
		{
			Data();
			~Data();

			D3D11_PRIMITIVE_TOPOLOGY mTopology;
			std::vector<graphics::Vertex> vertices;
			std::vector<UINT> indices;

		};

		Mesh();
		~Mesh();

		virtual HRESULT Save(const std::wstring& path)override;
		virtual HRESULT Load(const std::wstring& path) override;

		bool CreatVB(const std::vector<graphics::Vertex>& vertices);
		bool CreatIB(const std::vector<UINT>& indices);
		void Bind();

	private:

		graphics::VertexBuffer mVB;
		graphics::IndexBuffer mIB;

		Data mData;

	};

}
