#pragma once

#include <windows.h>
#include <gl/gl.h>
#include "Creatures.h"
#include "Environment.h"
#include "Map.h"
#include "library/stb_easy_font.h"
#define STB_IMAGE_IMPLEMENTATION
#include "library/stb_image.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);

//-------------------------VARIABLES---------------------------//
namespace Map
{
	Player player(100, 30, 0, 0, humanic, Coord(getCoordPlane(0), getCoordPlane(0)));
	//const int MAP_SIZE = SIZE;
	bool isPressed = false;
	char map[MAP_SIZE][MAP_SIZE];
	int floor[MAP_SIZE][MAP_SIZE];
	char enemys[MAP_SIZE][MAP_SIZE];
	Coord info(-1, -1);
	int inventoryInfo = -1;
	int w, h;
	int watchingIndex = 0;
	float fading = 0.0f;
	//BOT LOGIC
	int pathSize = 0, pathI = 0;
	Direction* pathToPlayer = nullptr;
	auto botLogicIterator = Enemy::getEnemys().begin();

	//TEXTURES
	unsigned int humanTexture;
	float textureMap[] = { 0,1, 1,1, 1,0, 0,0 };
	unsigned int icyTexture;
	unsigned int zooTexture;
	unsigned int zombieTexture;



	//FUN
	float tetha = 0.0f;
}

namespace Textures
{

}