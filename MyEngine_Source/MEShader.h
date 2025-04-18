#pragma once

#include "MEGraphicDevice_DX11.h"
#include "MEResource.h"

namespace ME::graphics
{


	class Shader: public Resource
	{
	public:

		Shader();
		virtual ~Shader();

		virtual HRESULT Save(const std::wstring& path) override;
		virtual HRESULT Load(const std::wstring& path) override;

		bool Create(const eShaderStage stage, const std::wstring& fileName);

		bool CreateVertexShader(const std::wstring& fileName);
		bool CreatePixelShader(const std::wstring& fileName);

		void Bind();




		[[nodiscard]]Microsoft::WRL::ComPtr<ID3DBlob> GetVSBlob() const{ return mVSBlob; }
		[[nodiscard]]Microsoft::WRL::ComPtr<ID3DBlob> GetHSBlob() const {return mHSBlob;}
		[[nodiscard]]Microsoft::WRL::ComPtr<ID3DBlob> GetGSBlob() const {return mGSBlob;}
		[[nodiscard]]Microsoft::WRL::ComPtr<ID3DBlob> GetDSBlob() const {return mDSBlob;}
		[[nodiscard]]Microsoft::WRL::ComPtr<ID3DBlob> GetPSBlob() const {return mPSBlob;}

		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11VertexShader>  GetVS() const { return mVS; }
		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11HullShader> GetHS() const { return mHS; }
		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11DomainShader> GetDS() const { return mDS; }
		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11GeometryShader> GetGS() const { return mGS; }
		[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPS() const { return mPS; }

		[[noreturn]]void SetRasterizerState(const eRasterizerState state) { mRasterizerState = state; }
		[[noreturn]]void SetBlendState(const eBlendState state) { mBlendState = state; }
		[[noreturn]]void SetDepthStencilState(const eDepthStencilState state) { mDepthStencilState = state; }

	private:

		static bool bWireFrame;

			Microsoft::WRL::ComPtr<ID3DBlob> mVSBlob;
			Microsoft::WRL::ComPtr<ID3DBlob> mHSBlob;
			Microsoft::WRL::ComPtr<ID3DBlob> mGSBlob;
			Microsoft::WRL::ComPtr<ID3DBlob> mDSBlob;
			Microsoft::WRL::ComPtr<ID3DBlob> mPSBlob;
		
			Microsoft::WRL::ComPtr<ID3D11VertexShader> mVS;
			Microsoft::WRL::ComPtr<ID3D11HullShader> mHS;
			Microsoft::WRL::ComPtr<ID3D11DomainShader> mDS;
			Microsoft::WRL::ComPtr<ID3D11GeometryShader> mGS;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> mPS;

			graphics::eRasterizerState mRasterizerState;
			graphics::eBlendState mBlendState;
			graphics::eDepthStencilState mDepthStencilState;


	};

}

