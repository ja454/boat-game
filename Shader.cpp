#include "Shader.h"

void VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC *layoutDesc, UINT numElements)
{
	D3DReadFileToBlob(shaderpath.c_str(), this->pBlob.GetAddressOf());
	
	device->CreateVertexShader(this->pBlob->GetBufferPointer(), this->pBlob->GetBufferSize(), NULL, this->pVertexShader.GetAddressOf());

	device->CreateInputLayout(layoutDesc, numElements, this->pBlob->GetBufferPointer(), this->pBlob->GetBufferSize(), this->pInputLayout.GetAddressOf());

	return;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->pVertexShader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return this->pBlob.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->pInputLayout.Get();
}


void PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath)
{
	D3DReadFileToBlob(shaderpath.c_str(), this->pBlob.GetAddressOf());
	
	device->CreatePixelShader(this->pBlob.Get()->GetBufferPointer(), this->pBlob.Get()->GetBufferSize(), NULL, this->pPixelShader.GetAddressOf());
	
	return;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->pPixelShader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->pBlob.Get();
}