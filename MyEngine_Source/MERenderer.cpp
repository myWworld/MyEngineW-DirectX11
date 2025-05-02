#include "MERenderer.h"
#include "MEGraphicDevice_DX11.h"

#include "MEResources.h"
#include "MEShader.h"

#include "MEMaterial.h"


namespace ME::renderer
{
	Camera* mainCamera = nullptr;
	Camera* playerCamera = nullptr;

	

	ConstantBuffer* constantBuffers[(UINT)graphics::eCBType::End] = {};

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[(UINT)eSamplerType::End] = {};
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[(UINT)eRasterizerState::End] = {};
	Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[(UINT)eBlendState::End] = {};
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilStates[(UINT)eDepthStencilState::End] = {};

	void LoadStates()
	{
#pragma region SamplerState

		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));


		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf());

		ZeroMemory(&samplerDesc, sizeof(samplerDesc));

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		GetDevice()->CreateSamplerState(&samplerDesc, samplerStates[(UINT)eSamplerType::PostProcess].GetAddressOf());

		GetDevice()->BindSamplers((UINT)eSamplerType::Point, 1, samplerStates[(UINT)eSamplerType::Point].GetAddressOf());
		GetDevice()->BindSamplers((UINT)eSamplerType::Linear, 1, samplerStates[(UINT)eSamplerType::Linear].GetAddressOf());
		GetDevice()->BindSamplers((UINT)eSamplerType::Anisotropic, 1, samplerStates[(UINT)eSamplerType::Anisotropic].GetAddressOf());
		GetDevice()->BindSamplers((UINT)eSamplerType::PostProcess, 1, samplerStates[(UINT)eSamplerType::PostProcess].GetAddressOf());

#pragma endregion

#pragma region rasterizer state

		D3D11_RASTERIZER_DESC rsDesc = {};

		rsDesc.AntialiasedLineEnable = false;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rsDesc.DepthBias = 0;
		rsDesc.DepthBiasClamp = 0.0f;
		rsDesc.DepthClipEnable = true;
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.FrontCounterClockwise = false;
		rsDesc.MultisampleEnable = false;
		rsDesc.ScissorEnable = false;
		rsDesc.SlopeScaledDepthBias = 0.0f;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[(UINT)eRasterizerState::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[(UINT)eRasterizerState::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[(UINT)eRasterizerState::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		GetDevice()->CreateRasterizerState(&rsDesc, rasterizerStates[(UINT)eRasterizerState::WireFrame].GetAddressOf());




#pragma endregion


#pragma region blend state

		D3D11_BLEND_DESC bsDesc = {};

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)eBlendState::AlphaBlend].GetAddressOf());

		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		GetDevice()->CreateBlendState(&bsDesc, blendStates[(UINT)eBlendState::OneOne].GetAddressOf());

#pragma endregion


#pragma region depthstencil state

		D3D11_DEPTH_STENCIL_DESC dsDesc = {};

		dsDesc.DepthEnable = true;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;

		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilStates[(UINT)eDepthStencilState::LessEqual].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.StencilEnable = false;
		GetDevice()->CreateDepthStencilState(&dsDesc, depthStencilStates[(UINT)eDepthStencilState::DepthNone].GetAddressOf());

#pragma endregion
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


		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[2] = {};
		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

		graphics::Shader* triangleShader = Resources::Find<graphics::Shader>(L"TriangleShader");
		mesh->SetVertexBufferParams(2, inputLayoutDesces, triangleShader->GetVSBlob()->GetBufferPointer(), triangleShader->GetVSBlob()->GetBufferSize());

		mesh->CreateIB(indices);
		mesh->CreateVB(vertexes);

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

		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[4] = {};
		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

		inputLayoutDesces[2].AlignedByteOffset = 28; //12 + 16
		inputLayoutDesces[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[2].InputSlot = 0;
		inputLayoutDesces[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[2].SemanticName = "NORMAL";
		inputLayoutDesces[2].SemanticIndex = 0;


		inputLayoutDesces[3].AlignedByteOffset = 40; //12 + 16
		inputLayoutDesces[3].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputLayoutDesces[3].InputSlot = 0;
		inputLayoutDesces[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[3].SemanticName = "TEXCOORD";
		inputLayoutDesces[3].SemanticIndex = 0;

		graphics::Shader* spriteShader = Resources::Find<graphics::Shader>(L"SpriteDefaultShader");
		mesh->SetVertexBufferParams(4, inputLayoutDesces, spriteShader->GetVSBlob()->GetBufferPointer(), spriteShader->GetVSBlob()->GetBufferSize());


		mesh->CreateIB(indices);
		mesh->CreateVB(vertexes);

		ME::Resources::Insert(L"RectMesh", mesh);
	}

	void LoadModels(Mesh* mesh)
	{
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[6] = {};
		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

		inputLayoutDesces[2].AlignedByteOffset = 28; 
		inputLayoutDesces[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[2].InputSlot = 0;
		inputLayoutDesces[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[2].SemanticName = "NORMAL";
		inputLayoutDesces[2].SemanticIndex = 0;


		inputLayoutDesces[3].AlignedByteOffset = 40; 
		inputLayoutDesces[3].Format = DXGI_FORMAT_R32G32B32A32_SINT;
		inputLayoutDesces[3].InputSlot = 0;
		inputLayoutDesces[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[3].SemanticName = "BONEINDICES";
		inputLayoutDesces[3].SemanticIndex = 0;


		inputLayoutDesces[4].AlignedByteOffset = 56; 
		inputLayoutDesces[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[4].InputSlot = 0;
		inputLayoutDesces[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[4].SemanticName = "BONEWEIGHTS";
		inputLayoutDesces[4].SemanticIndex = 0;

		inputLayoutDesces[5].AlignedByteOffset = 72; 
		inputLayoutDesces[5].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputLayoutDesces[5].InputSlot = 0;
		inputLayoutDesces[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[5].SemanticName = "TEXCOORD";
		inputLayoutDesces[5].SemanticIndex = 0;

		graphics::Shader* modelShader = Resources::Find<graphics::Shader>(L"ModelShader");
		mesh->SetVertexBufferParams(6, inputLayoutDesces, modelShader->GetVSBlob()->GetBufferPointer(), modelShader->GetVSBlob()->GetBufferSize());

	}

	void LoadStaticModels(Mesh* mesh)
	{
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesces[4] = {};
		inputLayoutDesces[0].AlignedByteOffset = 0;
		inputLayoutDesces[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[0].InputSlot = 0;
		inputLayoutDesces[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[0].SemanticName = "POSITION";
		inputLayoutDesces[0].SemanticIndex = 0;

		inputLayoutDesces[1].AlignedByteOffset = 12;
		inputLayoutDesces[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputLayoutDesces[1].InputSlot = 0;
		inputLayoutDesces[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[1].SemanticName = "COLOR";
		inputLayoutDesces[1].SemanticIndex = 0;

		inputLayoutDesces[2].AlignedByteOffset = 28; //12 + 16
		inputLayoutDesces[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesces[2].InputSlot = 0;
		inputLayoutDesces[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[2].SemanticName = "NORMAL";
		inputLayoutDesces[2].SemanticIndex = 0;


		inputLayoutDesces[3].AlignedByteOffset = 40; //12 + 16
		inputLayoutDesces[3].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputLayoutDesces[3].InputSlot = 0;
		inputLayoutDesces[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesces[3].SemanticName = "TEXCOORD";
		inputLayoutDesces[3].SemanticIndex = 0;

		graphics::Shader* modelShader = Resources::Find<graphics::Shader>(L"StaticModelShader");
		mesh->SetVertexBufferParams(4, inputLayoutDesces, modelShader->GetVSBlob()->GetBufferPointer(), modelShader->GetVSBlob()->GetBufferSize());

	}

	void LoadMeshes()
	{
		LoadTriangleMesh();
		LoadRectMesh();
	}

	void LoadShaders()
	{
		ME::Resources::Load<graphics::Shader>(L"TriangleShader", L"..\\Shaders_SOURCE\\Triangle");
		ME::Resources::Load<graphics::Shader>(L"SpriteDefaultShader", L"..\\Shaders_SOURCE\\SpriteDefault");
		ME::Resources::Load<graphics::Shader>(L"WireFrameShader", L"..\\Shaders_SOURCE\\WireFrame");
		ME::Resources::Load<graphics::Shader>(L"ModelShader", L"..\\Shaders_SOURCE\\Model");
		ME::Resources::Load<graphics::Shader>(L"StaticModelShader", L"..\\Shaders_SOURCE\\StaticModel");



	}

	void LoadMaterials()
	{
		Material* triangleMaterial = new Material();
		ME::Resources::Insert(L"TriangleMaterial", triangleMaterial);

		Material* spriteMaterial = new Material();
		ME::Resources::Insert(L"SpriteMaterial", spriteMaterial);

		Material* modelMaterial = new Material();
		ME::Resources::Insert(L"ModelMaterial", modelMaterial);

		Material* staticModelMaterial = new Material();
		ME::Resources::Insert(L"StaticModelMaterial", staticModelMaterial);

		spriteMaterial->SetShader(ME::Resources::Find <graphics::Shader>(L"SpriteDefaultShader"));
		triangleMaterial->SetShader(ME::Resources::Find <graphics::Shader>(L"TriangleShader"));
		modelMaterial->SetShader(ME::Resources::Find <graphics::Shader>(L"ModelShader"));
		staticModelMaterial->SetShader(ME::Resources::Find <graphics::Shader>(L"StaticModelShader"));
	}

	void LoadConstantBuffers()
	{
		constantBuffers[CBSLOT_TRANSFORM] = new ConstantBuffer(eCBType::Transform);
		constantBuffers[CBSLOT_TRANSFORM]->Create(sizeof(TransformCB));
		constantBuffers[CBSLOT_ANIMATION] = new ConstantBuffer(eCBType::Animation);
		constantBuffers[CBSLOT_ANIMATION]->Create(sizeof(AnimationCB));

	}

	void Initialize()
	{
		LoadStates();
		LoadShaders();
		LoadMeshes();
		LoadMaterials();
		LoadConstantBuffers();
	}

	void Release()
	{
		for (int i = 0; i < (UINT)eCBType::End; i++)
		{
			delete constantBuffers[i];
			constantBuffers[i] = nullptr;
		}
		
	}
}