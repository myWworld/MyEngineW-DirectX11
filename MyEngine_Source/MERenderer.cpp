#include "MERenderer.h"
#include "MEGraphicDevice_DX11.h"

#include "MEResources.h"
#include "MEShader.h"
#include "MEMesh.h"

#include "MEMaterial.h"


namespace ME::renderer
{
	Camera* mainCamera = nullptr;
	Camera* playerCamera = nullptr;

	

	ConstantBuffer constantBuffers[(UINT)graphics::eCBType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};

	extern Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;


	void LoadStage()
	{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::End].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::End].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));


		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::End].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::End].GetAddressOf());

		GetDevice()->BindSamplers((UINT)eSamplerType::Point, 1, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSamplers((UINT)eSamplerType::Linear, 1, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf());
		GetDevice()->BindSamplers((UINT)eSamplerType::Anisotropic, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSamplers((UINT)eSamplerType::PostProcess, 1, samplerStates[(UINT)eSamplerType::PostProcess].GetAddressOf());

	}
	
	void LoadTriangleMesh()
	{
		Mesh* mesh = new Mesh();

		//xyz
	//rgba

		std::vector<graphics::Vertex> vertexes = {};
		std::vector<UINT> indices;

		vertexes.resize(3);

		vertexes[0].pos = Vector3(0.f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);

		vertexes[1].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[1].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

		vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		mesh->CreatIB(indices);
		mesh->CreatVB(vertexes);

		ME::Resources::Insert(L"TriangleMesh", mesh);
	}

	void LoadRectMesh()
	{
		Mesh* mesh = new Mesh();

		std::vector<graphics::Vertex> vertexes = {};
		std::vector<UINT> indices;

		vertexes.resize(4);

		vertexes[0].pos = Vector3(-0.5f, 0.5f, 0.0f);
		vertexes[0].color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		vertexes[0].uv = Vector2(0.0f, 0.0f);

		vertexes[1].pos = Vector3(0.5f, 0.5f, 0.0f);
		vertexes[1].color = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		vertexes[1].uv = Vector2(1.0f, 0.0f);

		vertexes[2].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[2].pos = Vector3(0.5f, -0.5f, 0.0f);
		vertexes[2].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[2].uv = Vector2(1.0f, 1.0f);

		vertexes[3].pos = Vector3(-0.5f, -0.5f, 0.0f);
		vertexes[3].color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
		vertexes[3].uv = Vector2(0.0f, 1.0f);

		indices.push_back(0);
		indices.push_back(2);
		indices.push_back(3);

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);

		mesh->CreatIB(indices);
		mesh->CreatVB(vertexes);

		ME::Resources::Insert(L"RectMesh", mesh);
	}

	void LoadMeshes()
	{
		LoadTriangleMesh();
		LoadRectMesh();
	}

	void LoadShaders()
	{
		ME::Resources::Load<graphics::Shader>(L"TriangleShader", L"..\\Shaders_SOURCE\\Triangle");
		ME::Resources::Load<graphics::Shader>(L"SpriteShader", L"..\\Shaders_SOURCE\\Sprite");
	}

	void LoadMaterials()
	{
		Material* spriteMaterial = new Material();
		ME::Resources::Insert(L"SpriteMaterial", spriteMaterial);

		spriteMaterial->SetShader(ME::Resources::Find <graphics::Shader>(L"SpriteShader"));
	}

	void LoadConstantBuffers()
	{
		constantBuffers[(UINT)graphics::eCBType::Transform].Create(graphics::eCBType::Transform, sizeof(Vector4));
	}

	void Initialize()
	{
		LoadStage();
		LoadMeshes();
		LoadShaders();
		LoadMaterials();
		LoadConstantBuffers();
	}

	void Release()
	{
		//if(inputLayouts)
			//inputLayouts->Release();
		
	}
}