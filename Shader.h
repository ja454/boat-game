#pragma once
#include <iostream>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl.h>

#pragma comment(lib, "D3DCompiler.lib")

class PixelShader
{
public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> pBlob;
};


class VertexShader
{
public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shaderpath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT numElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};