#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <iostream>
#include <DirectXMath.h>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include "Graphics.h"
#include <d3dcompiler.h>
#include <random>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

class Sprite
{

public:

	Sprite(UINT type, Microsoft::WRL::ComPtr<ID3D11Device> pDevice);
	void SetScale(float x, float y);
	void SetPos(float x, float y);
	void SetOrigin(float x, float y);
	void SetRot(float rotation);
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext);
	DirectX::XMFLOAT2 position;
	float width;
	float height;
	float throttle;
	float rotation;
	RECT imageDimensions;

private:

	DirectX::XMFLOAT2 scale;
	DirectX::XMFLOAT2 origin;

	//RECT imageDimensions;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTexture;

};