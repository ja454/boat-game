#include <Windows.h>
#include "Graphics.h"
#include "Game.h"
#include "Sprite.h"
#include "Text.h"
#include <iostream>
#include <cmath>
#include "pch.h"
#include <chrono>
#pragma comment(lib, "DirectXTK.lib")

#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )
#define SINGLEPLAYER 0
#define MULTIPLAYER 1

WNDCLASSEX window(HINSTANCE& hInstance, HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int& nCmdShow);

void windowInitialization();

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{

	case WM_CLOSE:
		PostQuitMessage(21);
		break;

	case WM_QUIT:
		PostQuitMessage(4);
		break;

	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wc = window(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	
	HWND hWnd = CreateWindowEx(0, wc.lpszClassName, L"Boat Battle", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX, 0, 0, 1200, 800, nullptr, nullptr, hInstance, nullptr);
	
	Graphics gfx(hWnd);

	ShowWindow(hWnd, SW_SHOW);

	MSG msg;

	CoInitialize(NULL);

	RECT rect;
	GetWindowRect(hWnd, &rect);

	World game(SINGLEPLAYER, gfx);
	//World game(MULTIPLAYER, gfx);

	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;
	game.spriteObject[BACKGROUND]->SetScale(width/game.spriteObject[BACKGROUND]->width, height/game.spriteObject[BACKGROUND]->height);

	auto lastTime = std::chrono::steady_clock::now();
	auto startTime = std::chrono::steady_clock::now();
	std::string txt = "Hello World";
	Text Hello(txt);

	while(1)
	{
		//GetMessage(&msg, nullptr, 0, 0);
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				return TRUE;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		auto current = std::chrono::steady_clock::now();
		std::string elapsed_string = std::to_string(std::chrono::duration<double>(current - lastTime).count()) + 's';
		txt = std::to_string(game.spriteObject[PLAYER_1]->position.x);
		txt += "," + std::to_string(game.spriteObject[PLAYER_1]->position.y) + "\n Time = " + elapsed_string;
		txt += "\n Score: " + std::to_string(game.score);
		txt += "\n Ammo: " + std::to_string(game.ammo);
		std::string currentTime = std::to_string(int(std::chrono::duration<double>(current - startTime).count())) + 's';
		txt += "\n Time: " + currentTime;
		Hello.SetText(txt);
		gfx.ClearBuffer(0.0f, 0.0f, 0.0f);
		game.input();
		game.torpedoMovement();
		game.update();
		Hello.Draw(gfx.pContext, gfx.pDevice);
		gfx.EndFrame();
		//if (int(std::chrono::duration<double>(current - startTime).count()) > 10) { break; }
		lastTime = current;
	}

	return msg.wParam;
}

WNDCLASSEX window(HINSTANCE &hInstance, HINSTANCE &hPrevInstance, LPSTR &lpCmdLine, int &nCmdShow)
{

	const auto pClassName = L"Game";

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = pClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	return wc;
}
