#include "MERenderer.h"
#include "MEGraphicDevice_DX11.h"

#include "MEResources.h"
#include "MEShader.h"


namespace ME::renderer
{
	Camera* mainCamera = nullptr;
	Camera* playerCamera = nullptr;

	std::vector<graphics::Vertex> vertexes = {};
	std::vector<UINT> indices;

	Mesh* mesh = nullptr;

	graphics::ConstantBuffer constantBuffers[(UINT)graphics::eCBType::End] = {};

	ID3D11Buffer* constantBuffer = nullptr;
	ID3D11InputLayout* inputLayouts = nullptr;

	void LoadTriangleMesh()
	{
		mesh = new Mesh();

		//xyz
	//rgba
		renderer::vertexes.resize(3);

		renderer::vertexes[0].pos = Vector3(0.f, 0.5f, 0.0f);
		renderer::vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		renderer::vertexes[1].pos = Vector3(0.5f, -0.5f, 0.0f);
		renderer::vertexes[1].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		renderer::vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.0f);
		renderer::vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		mesh->CreatIB(indices);
		mesh->CreatVB(vertexes);
	}

	void LoadMeshes()
	{
		LoadTriangleMesh();
	}

	void LoadShaders()
	{
		ME::Resources::Load<graphics::Shader>(L"TriangleShader", L"..\\Shaders_SOURCE\\Triangle");
	}

	void LoadConstantBuffers()
	{
		constantBuffers[(UINT)graphics::eCBType::Transform].Create(graphics::eCBType::Transform, sizeof(Vector4));
	}

	void Initialize()
	{
		LoadMeshes();
		LoadShaders();
		LoadConstantBuffers();
	}

	void Release()
	{
	
		inputLayouts->Release();
		delete mesh;
	}
}