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
#include <Windows.h>
#include "Sprite.h"
#include <cmath>
#include <random>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )

#define SINGLEPLAYER 0
#define MULTIPLAYER 1

#define TOPLEFT 1
#define TOPRIGHT 2
#define BOTTOMLEFT 3
#define BOTTOMRIGHT 4

#define BACKGROUND 0
#define PLAYER_1 1
#define PLAYER_2 2
#define COIN_1 3
#define COIN_2 4
#define COIN_3 5
#define COIN_4 6
#define COIN_5 7
#define TORPEDO_MAIN 8
#define TORPEDO_ENEMY 9

void islandCollision(float& x, float& y);

//void coinCollision(float& x, float& y);

class World
{
public:

	World(bool multiPlayer, Graphics &gfx);
	~World();
	void input();
	void update();
	void coinCollision(float& x, float& y);
	void fireTorpedo();
	void objectMovement();
	void torpedoCollision();
	void playerCollision();

	std::unique_ptr<Sprite> spriteObject[10];
	std::unique_ptr<Graphics> gfx;
	int score;
	int lives;
	int ammo = 0;
	int quadrant;

private:

};
