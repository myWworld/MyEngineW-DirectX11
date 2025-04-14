#pragma once

#include "MEResource.h"
#include "MEVertexBuffer.h"
#include "MEIndexBuffer.h"
#include "MEInputLayout.h"


namespace ME
{
	class Mesh: public Resource
	{
	public:


		struct MeshData
		{
			MeshData();
			~MeshData();

			D3D11_PRIMITIVE_TOPOLOGY mTopology;
			std::vector<graphics::Vertex> vertices;
			std::vector<UINT> indices;

		};

		Mesh();
		Mesh(std::vector<graphics::Vertex> vertices, std::vector<unsigned int> indicies);
		~Mesh();

		virtual HRESULT Save(const std::wstring& path)override;
		virtual HRESULT Load(const std::wstring& path) override;

		bool CreateVB(const std::vector<graphics::Vertex>& vertices);
		bool CreateIB(const std::vector<UINT>& indices);
		void SetVertexBufferParams(UINT vertexCount, D3D11_INPUT_ELEMENT_DESC* layout, const void* pShaderByteCodeWithInputSignature
			, SIZE_T BytecodeLength);
		void Bind();

		UINT GetIndexCount() const { return mIB.GetIndexCount(); }


	private:

		graphics::InputLayout mInputLayout;
		graphics::VertexBuffer mVB;
		graphics::IndexBuffer mIB;

		MeshData mData;

	};

}
