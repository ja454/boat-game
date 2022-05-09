#include "Sprite.h"

Sprite::Sprite(UINT type, Microsoft::WRL::ComPtr<ID3D11Device> pDevice)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist1(1.0, 1200.0);
	std::uniform_real_distribution<float> dist2(1.0, 800.0);

	switch (type) {

	case 1:

		imageDimensions.left = 0;
		imageDimensions.top = 0;
		imageDimensions.right = 1022;
		imageDimensions.bottom = 703;
		width = imageDimensions.right - imageDimensions.left;
		height = imageDimensions.bottom - imageDimensions.top;
		SetScale(1, 1);
		SetPos(0, 0);
		SetRot(0);
		SetOrigin(0, 0);
		DirectX::CreateWICTextureFromFile(pDevice.Get(), L"Data\\Textures\\gamebackgroud.png", nullptr, pTexture.GetAddressOf());
		break;

	case 2:

		imageDimensions.left = 0;
		imageDimensions.top = 0;
		imageDimensions.right = 860;
		imageDimensions.bottom = 322;
		width = imageDimensions.right - imageDimensions.left;
		height = imageDimensions.bottom - imageDimensions.top;
		SetScale(0.125, 0.125);
		SetPos(300, 300);
		SetOrigin(imageDimensions.right/2, imageDimensions.bottom/2);
		SetRot(0);
		DirectX::CreateWICTextureFromFile(pDevice.Get(), L"Data\\Textures\\boat.png", nullptr, pTexture.GetAddressOf());
		break;

	case 3:

		imageDimensions.left = 0;
		imageDimensions.top = 0;
		imageDimensions.right = 500;
		imageDimensions.bottom = 500;
		width = imageDimensions.right - imageDimensions.left;
		height = imageDimensions.bottom - imageDimensions.top;
		SetScale(0.05, 0.05);
		SetPos(dist1(mt), dist2(mt));
		SetOrigin(imageDimensions.right / 2, imageDimensions.bottom / 2);
		SetRot(0);
		DirectX::CreateWICTextureFromFile(pDevice.Get(), L"Data\\Textures\\coin.png", nullptr, pTexture.GetAddressOf());
		break;

	case 4:

		imageDimensions.left = 0;
		imageDimensions.top = 0;
		imageDimensions.right = 690;
		imageDimensions.bottom = 110;
		width = imageDimensions.right - imageDimensions.left;
		height = imageDimensions.bottom - imageDimensions.top;
		SetScale(0.1, 0.1);
		SetPos(1, 1);
		SetOrigin(imageDimensions.right/2, imageDimensions.bottom/2);
		SetRot(0);
		DirectX::CreateWICTextureFromFile(pDevice.Get(), L"Data\\Textures\\torpedo.png", nullptr, pTexture.GetAddressOf());
		break;

	}

	return;
}

void Sprite::SetScale(float x, float y)
{
	this->scale.x = x;
	this->scale.y = y;
	return;
}

void Sprite::SetPos(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
	return;
}

void Sprite::SetOrigin(float x, float y)
{
	this->origin.x = x;
	this->origin.y = y;
	return;
}

void Sprite::SetRot(float rotation)
{
	this->rotation = rotation;
	return;
}


void Sprite::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext)
{
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch(new DirectX::SpriteBatch(pContext.Get()));

	spriteBatch->Begin();
	spriteBatch->Draw(pTexture.Get(), position, &imageDimensions, DirectX::Colors::White, rotation, origin, scale);
	spriteBatch->End();
}