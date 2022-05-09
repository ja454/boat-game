#pragma once
#include "Windows.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <iostream>
#include <DirectXMath.h>
#include "Shader.h"
#include <SpriteFont.h>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>

class Graphics
{
public:

	Graphics(HWND hWnd);
	~Graphics();
	void EndFrame();
	void ClearBuffer(float red, float green, float blue);
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;

private:

	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;

};