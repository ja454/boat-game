#include "Text.h"


Text::Text(std::string text)
{

	SetScale(1, 1);
	SetPos(0, 0);
	SetOrigin(0, 0);
	SetRot(0);
	this->text = text;
		
	return;
}

void Text::SetScale(float x, float y)
{
	this->scale.x = x;
	this->scale.y = y;
	return;
}

void Text::SetPos(float x, float y)
{
	this->position.x = x;
	this->position.y = y;
	return;
}

void Text::SetOrigin(float x, float y)
{
	this->origin.x = x;
	this->origin.y = y;
	return;
}

void Text::SetRot(float rotation)
{
	this->rotation = rotation;
	return;
}

void Text::SetText(std::string text)
{
	this->text = text;
	return;
}


void Text::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext, Microsoft::WRL::ComPtr<ID3D11Device> pDevice)
{
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch(new DirectX::SpriteBatch(pContext.Get()));
	std::unique_ptr<DirectX::SpriteFont>  spriteFont(new DirectX::SpriteFont(pDevice.Get(), L"Data\\comic_sans_ms_16.spritefont"));

	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), text.c_str(), position, DirectX::Colors::White, rotation, origin, scale);
	spriteBatch->End();
}