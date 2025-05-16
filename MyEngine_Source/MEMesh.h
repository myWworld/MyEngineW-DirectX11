#pragma once

#include "MEResource.h"
#include "MEVertexBuffer.h"
#include "MEIndexBuffer.h"
#include "MEInputLayout.h"
#include "METexture.h"
#include "MEBone.h"


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
			std::vector<Bone> bones;

		};

		Mesh();
		Mesh(std::vector<graphics::Vertex> &vertices, std::vector<unsigned int>& indicies, std::vector<Bone>& bones);
		~Mesh();

		virtual HRESULT Save(const std::wstring& path)override;
		virtual HRESULT Load(const std::wstring& path) override;

		bool CreateVB(const std::vector<graphics::Vertex>& vertices);
		bool CreateIB(const std::vector<UINT>& indices);
		void SetVertexBufferParams(UINT vertexCount, D3D11_INPUT_ELEMENT_DESC* layout, const void* pShaderByteCodeWithInputSignature
			, SIZE_T BytecodeLength);
		void Bind();

		UINT GetIndexCount() const { return mIB.GetIndexCount(); }

		void SetDiffuseTexture(graphics::Texture* texture) { diffuseTexture = texture; }
		void SetSpecularTexture(graphics::Texture* texture) { specularTexture = texture; }
		void SetNormalTexture(graphics::Texture* texture) { normalTexture = texture; }

		graphics::Texture* GetDiffuseTexture() { return diffuseTexture; }
		graphics::Texture* GetSpecularTexture() { return specularTexture; }
		graphics::Texture* GetNormalTexture() { return normalTexture; }

		void SetSkinned(bool skinned) { mbIsSkinned = skinned; }
		bool IsSkinned() { return mbIsSkinned; }


	private:

		graphics::InputLayout mInputLayout;
		graphics::VertexBuffer mVB;
		graphics::IndexBuffer mIB;

		graphics::Texture* diffuseTexture;
		graphics::Texture* specularTexture;
		graphics::Texture* normalTexture;

		bool mbIsSkinned;

		MeshData mData;

	};

}
