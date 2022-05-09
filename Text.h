#pragma once
#include <string>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <iostream>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include "Graphics.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class Text
{

public:

	Text(std::string text);
	void SetScale(float x, float y);
	void SetPos(float x, float y);
	void SetOrigin(float x, float y);
	void SetRot(float rotation);
	void SetText(std::string text);
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, Microsoft::WRL::ComPtr<ID3D11Device> pDevice);

private:

	float rotation;

	std::string text;

	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 origin;

	RECT imageDimensions;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;

};