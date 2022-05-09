#pragma once
#include <DirectXMath.h>

struct Vertex {

	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 textCoord;

	Vertex(float x, float y, float u, float v)
		: pos(x, y), textCoord(u, v) {}
	
};