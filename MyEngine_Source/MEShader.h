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


		Microsoft::WRL::ComPtr<ID3DBlob> GetVSBloc(){ return mVSBlob; }

	private:

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
		
	};

}

