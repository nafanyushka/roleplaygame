#pragma once

#include <iostream>
#include <windows.h>
#include <fstream>
#include <gl/gl.h>
#include "Creatures.h"
#include "Environment.h"
#include "Map.h"
#include "library/stb_easy_font.h"
//#include "library/irrklang/irrKlang.h"
#include <irrKlang.h>
#define STB_IMAGE_IMPLEMENTATION
#include "library/stb_image.h"
using namespace irrklang;
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


void printString(float x, float y, char* text, float r, float g, float b);
void drawSquare(float size, float x, float y);
void drawRectangle(float h, float l, float x, float y);


//-------------------------VARIABLES---------------------------//
namespace Map
{
	Player player(100, 10, 0, 0, humanic, Coord(getCoordPlane(1), getCoordPlane(1)));
	//const int MAP_SIZE = SIZE;
	bool isPressed = false;
	char map[MAP_SIZE][MAP_SIZE];
	int floor[MAP_SIZE][MAP_SIZE];
	char enemys[MAP_SIZE][MAP_SIZE];
	Coord info(-1, -1);
	ExitDoor* exit = nullptr;
	int inventoryInfo = -1;
	int equipmentInfo = -1;
	int w, h;
	int watchingIndex = 0;
	bool isEnd = false;
	float fading = 0.0f;
	//BOT LOGIC
	int pathSize = 0, pathI = 0;
	Direction* pathToPlayer = nullptr;
	auto botLogicIterator = Enemy::getEnemys().begin();
	Scene scene = Scene::menu;
	ISoundEngine* soundEngine = nullptr;
	int animCounter = 0;
	//TEXTURES
	float textureMap[] = { 0,1, 1,1, 1,0, 0,0 };
	unsigned int humanTexture;
	unsigned int banditTexture;
	unsigned int icyTexture;
	unsigned int zooTexture;
	unsigned int zombieTexture;
	unsigned int demonicTexture;
	unsigned int wallTexture;
	unsigned int doorTexture;
	unsigned int closedDoorTexture;
	unsigned int chestTexture;
	unsigned int closedChestTexture;
	unsigned int agilityTexture;
	unsigned int powerTexture;
	unsigned int intelligenceTexture;
	unsigned int potionTexture;
	unsigned int swordTexture;
	unsigned int masterkeyTexture;
	unsigned int helmetTexture;
	unsigned int pantsTexture;
	unsigned int bootsTexture;
	unsigned int bibTexture;
	unsigned int kalikTexture;
	unsigned int inventoryTexture;

	unsigned int newbtn;
	unsigned int loadbtn;
	unsigned int exitbtn;
	unsigned int menu;

	//FUN
	float tetha = 0.0f;
}

inline void createRoom(int fromX, int fromY, int toX, int toY)
{
	for (int i = fromX; i < toX; i++)
	{
		Map::map[i][fromY] = '#';
		Map::map[i][toY] = '#';
	}
	for (int j = fromY; j <= toY; j++)
	{
		Map::map[fromX][j] = '#';
		Map::map[toX][j] = '#';
	}
}

inline void createWall(int fromX, int fromY, int toX, int toY)
{
	for (int i = fromX; i <= toX; i++)
	{
		for (int j = fromY; j <= toY; j++)
		{
			Map::map[i][j] = '#';
		}
	}
}

inline void createMap()
{
	using namespace Map;
	//TEST!
	//player.setAgility(40);
	//TEST!
	//Map::player = Player(100, 10, 0, 0, humanic, Coord(getCoordPlane(1), getCoordPlane(1)));
	srand(time(NULL));
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++)
		{
			map[i][j] = '.';
			enemys[i][j] = '-';
			Map::floor[i][j] = rand() % 10;
		}
	}

	//creating walls
	createWall(0, 68, 31, 99);
	createWall(31, 74, 99, 99);
	createWall(78, 0, 99, 74);
	createWall(76, 0, 22, 74);
	createWall(2, 39, 12, 62);
	createWall(16, 39, 19, 66);
	createWall(2, 23, 4, 34);
	createWall(5, 25, 12, 34);
	createWall(13, 31, 22, 34);
	createWall(24, 25, 26, 35);
	createWall(31, 8, 33, 17);
	createWall(30, 13, 30, 17);
	createWall(35, 7, 40, 40);
	createWall(41, 15, 47, 40);
	createWall(48, 26, 76, 40);
	createWall(55, 41, 76, 64);
	createWall(63, 65, 76, 72);
	createWall(43, 4, 62, 6);
	createWall(61, 7, 62, 23);
	createWall(31, 49, 40, 53);
	createWall(43, 40, 51, 49);
	createWall(25, 56, 25, 59);

	//creating rooms
	createRoom(0, 0, 99, 99);
	createRoom(0, 0, 7, 5);
	createRoom(7, 0, 11, 13);
	createRoom(0, 5, 7, 13);
	createRoom(11, 7, 14, 11);
	createRoom(0, 13, 4, 23);
	createRoom(0, 23, 2, 68);
	createRoom(4, 13, 16, 21);
	createRoom(6, 13, 14, 19);
	createRoom(4, 21, 19, 25);
	createRoom(19, 21, 30, 25);
	createRoom(16, 13, 21, 21);
	createRoom(21, 13, 30, 21);
	createRoom(30, 17, 33, 25);
	createRoom(11, 5, 18, 13);
	createRoom(18, 5, 23, 13);
	createRoom(11, 0, 23, 5);
	createRoom(23, 0, 27, 8);
	createRoom(23, 8, 31, 13);
	createRoom(27, 5, 33, 8);
	createRoom(27, 0, 33, 5);
	createRoom(33, 0, 35, 44);
	createRoom(12, 25, 22, 31);
	createRoom(22, 25, 24, 34);
	createRoom(2, 39, 21, 68);
	createRoom(2, 39, 16, 66);
	createRoom(2, 34, 24, 39);
	createRoom(21, 39, 24, 49);
	createRoom(21, 49, 24, 68);
	createRoom(24, 35, 26, 49);
	createRoom(26, 25, 33, 44);
	createRoom(25, 49, 31, 56);
	createRoom(26, 56, 30, 59);
	createRoom(25, 59, 31, 68);
	createRoom(25, 59, 31, 68);
	createRoom(31, 53, 40, 64);
	createRoom(31, 53, 33, 64);
	createRoom(31, 64, 74, 64);
	createRoom(37, 66, 57, 74);
	createRoom(40, 57, 54, 64);
	createRoom(45, 57, 49, 64);
	createRoom(26, 44, 40, 49);
	createRoom(40, 41, 54, 57);
	createRoom(43, 49, 51, 57);
	createRoom(35, 0, 43, 7);
	createRoom(40, 7, 47, 15);
	createRoom(47, 6, 55, 23);
	createRoom(47, 12, 55, 18);
	createRoom(52, 18, 55, 23);
	createRoom(65, 11, 73, 26);
	createRoom(47, 23, 65, 26);
	createRoom(62, 4, 65, 23);
	createRoom(65, 13, 73, 19);
	createRoom(68, 13, 71, 16);
	createRoom(68, 16, 70, 19);
	createRoom(68, 0, 76, 4);
	createRoom(70, 4, 73, 11);
	createRoom(65, 6, 68, 11);
	createRoom(65, 2, 68, 6);
	createRoom(57, 2, 65, 4);
	createRoom(43, 0, 55, 4);
	createRoom(73, 4, 76, 26);
	createRoom(77, 0, 77, 22);

	Map::exit = new ExitDoor(47, 53);
	new Door(Coord(7, 3), true, 1);
	new Door(Coord(7, 9), false, 1);
	new Door(Coord(9, 13), false, 1);
	new Door(Coord(11, 9), false, 1);
	new Door(Coord(5, 13), false, 1);
	new Door(Coord(15, 13), false, 1);
	new Door(Coord(16, 17), false, 1);
	new Door(Coord(4, 17), false, 1);
	new Door(Coord(9, 19), false, 1);
	new Door(Coord(9, 21), false, 1);
	new Door(Coord(13, 25), false, 1);
	new Door(Coord(15, 5), false, 1);
	new Door(Coord(16, 17), false, 1);
	new Door(Coord(22, 28), false, 1);
	new Door(Coord(23, 34), false, 1);
	new Door(Coord(24, 37), false, 1);
	new Door(Coord(2, 37), false, 1);
	new Door(Coord(26, 42), false, 1);
	new Door(Coord(14, 39), false, 1);
	new Door(Coord(26, 47), false, 1);
	new Door(Coord(28, 49), false, 1);
	new Door(Coord(34, 44), false, 1);
	new Door(Coord(33, 28), false, 1);
	new Door(Coord(30, 24), false, 1);
	new Door(Coord(24, 21), false, 1);
	new Door(Coord(22, 13), false, 1);
	new Door(Coord(25, 8), false, 1);
	new Door(Coord(29, 8), false, 1);
	new Door(Coord(27, 3), false, 1);
	new Door(Coord(23, 3), false, 1);
	new Door(Coord(33, 7), false, 1);
	new Door(Coord(35, 4), false, 1);
	new Door(Coord(42, 7), false, 1);
	new Door(Coord(47, 14), false, 1);
	new Door(Coord(54, 18), false, 1);
	new Door(Coord(52, 22), false, 1);
	new Door(Coord(48, 23), false, 1);
	new Door(Coord(55, 22), false, 1);
	new Door(Coord(63, 23), false, 1);
	new Door(Coord(65, 25), false, 1);
	new Door(Coord(73, 25), false, 1);
	new Door(Coord(76, 25), false, 1);
	new Door(Coord(69, 19), false, 1);
	new Door(Coord(68, 17), false, 1);
	new Door(Coord(70, 17), false, 1);
	new Door(Coord(68, 14), false, 1);
	new Door(Coord(72, 13), false, 1);
	new Door(Coord(69, 11), false, 1);
	new Door(Coord(66, 11), false, 1);
	new Door(Coord(70, 9), false, 1);
	new Door(Coord(68, 7), false, 1);
	new Door(Coord(67, 6), false, 1);
	new Door(Coord(73, 5), false, 1);
	new Door(Coord(68, 3), false, 1);
	new Door(Coord(65, 3), false, 1);
	new Door(Coord(68, 1), false, 1);
	new Door(Coord(55, 2), false, 1);
	new Door(Coord(24, 37), false, 1);
	new Door(Coord(21, 43), false, 1);
	new Door(Coord(21, 52), false, 1);
	new Door(Coord(2, 64), false, 1);
	new Door(Coord(2, 67), false, 1);
	new Door(Coord(21, 67), false, 1);
	new Door(Coord(28, 56), false, 1);
	new Door(Coord(31, 55), false, 1);
	new Door(Coord(31, 61), false, 1);
	new Door(Coord(31, 66), false, 1);
	new Door(Coord(34, 64), false, 1);
	new Door(Coord(37, 65), false, 1);
	new Door(Coord(57, 65), false, 1);
	new Door(Coord(63, 73), false, 1);
	new Door(Coord(76, 73), false, 1);
	new Door(Coord(43, 64), false, 1);
	new Door(Coord(51, 64), false, 1);
	new Door(Coord(45, 60), false, 1);
	new Door(Coord(49, 60), false, 1);
	new Door(Coord(41, 57), false, 1);
	new Door(Coord(53, 57), false, 1);
	new Door(Coord(47, 57), false, 1);
	new Door(Coord(47, 66), false, 1);



	for (auto i = EnvironmentContainer::getMap().begin(); i != EnvironmentContainer::getMap().end(); i++)
	{
		int x = i->second->getCoord()->x;
		int y = i->second->getCoord()->y;
		if (i->second == Map::exit)
			map[x][y] = '?';
		else map[x][y] = '!';
	}
	new Chest(Coord(2, 4), new Item(masterkey), false, 1);
	new Chest(Coord(3, 4), new Weapon(10), false, 1);
	new Chest(Coord(4, 4), new Protection(helmet, 3), true, 2);
	new Chest(Coord(5, 4), new Protection(bib, 5), true, 2);
	new Chest(Coord(13, 9), new Protection(pants, 10), true, 3);
	new Chest(Coord(1, 9), new Item(masterkey), true, 1);
	new Chest(Coord(1, 4), new Potion(30, hp), true, 2);

	for (auto i = EnvironmentContainer::getMap().begin(); i != EnvironmentContainer::getMap().end(); i++)
	{
		int x = i->second->getCoord()->x;
		int y = i->second->getCoord()->y;
		if (map[x][y] != '!' && map[x][y] != '?')
		{
			map[x][y] = 'c';
		}
	}


	//ZOMBIE REGION

	new Enemy(30, 10, 0, 1, zombie, Coord(9, 3));
	new Enemy(30, 10, 0, 1, zombie, Coord(9, 11));
	new Enemy(30, 10, 0, 2, zombie, Coord(12, 9));
	new Enemy(30, 10, 0, 2, zombie, Coord(17, 3));
	new Enemy(30, 10, 0, 2, zombie, Coord(26, 1));
	new Enemy(30, 10, 0, 2, zombie, Coord(30, 3));
	new Enemy(30, 10, 0, 2, zombie, Coord(30, 6));
	new Enemy(30, 10, 0, 2, zombie, Coord(30, 12));
	new Enemy(30, 10, 0, 2, zombie, Coord(19, 7));
	new Enemy(30, 10, 0, 2, zombie, Coord(22, 7));
	new Enemy(30, 10, 0, 2, zombie, Coord(20, 9));
	new Enemy(30, 10, 0, 2, zombie, Coord(21, 9));
	new Enemy(30, 10, 0, 2, zombie, Coord(27, 15));
	new Enemy(30, 10, 0, 2, zombie, Coord(18, 15));
	new Enemy(30, 10, 0, 2, zombie, Coord(18, 19));
	new Enemy(30, 10, 0, 2, zombie, Coord(15, 14));
	new Enemy(30, 10, 0, 2, zombie, Coord(10, 16));
	new Enemy(30, 10, 0, 2, zombie, Coord(2, 15));
	new Enemy(30, 10, 0, 2, zombie, Coord(1, 18));
	new Enemy(30, 10, 0, 2, zombie, Coord(2, 21));
	new Enemy(30, 10, 0, 2, zombie, Coord(5, 24));
	new Enemy(30, 10, 0, 2, zombie, Coord(7, 22));
	new Enemy(30, 10, 0, 2, zombie, Coord(11, 24));
	new Enemy(30, 10, 0, 2, zombie, Coord(15, 22));
	new Enemy(30, 10, 0, 2, zombie, Coord(32, 24));

	////ZOO REGION

	new Enemy(30, 10, 0, 2, zoo, Coord(18, 27));
	new Enemy(30, 10, 0, 2, zoo, Coord(23, 30));
	new Enemy(30, 10, 0, 2, zoo, Coord(19, 37));
	new Enemy(30, 10, 0, 2, zoo, Coord(11, 37));
	new Enemy(30, 10, 0, 2, zoo, Coord(15, 36));
	new Enemy(30, 10, 0, 2, zoo, Coord(6, 36));
	new Enemy(30, 10, 0, 2, zoo, Coord(14, 49));
	new Enemy(30, 10, 0, 2, zoo, Coord(14, 43));
	new Enemy(30, 10, 0, 2, zoo, Coord(5, 64));
	new Enemy(30, 10, 0, 2, zoo, Coord(11, 64));
	new Enemy(30, 10, 0, 2, zoo, Coord(20, 67));
	new Enemy(30, 10, 0, 2, zoo, Coord(22, 52));

	////HUMAN REGION

	new Enemy(30, 10, 0, 2, humanic, Coord(38, 4));
	new Enemy(30, 10, 0, 2, humanic, Coord(41, 2));
	new Enemy(30, 10, 0, 2, humanic, Coord(43, 10));
	new Enemy(30, 10, 0, 2, humanic, Coord(45, 13));
	new Enemy(30, 10, 0, 2, humanic, Coord(48, 2));
	new Enemy(30, 10, 0, 2, humanic, Coord(49, 9));
	new Enemy(30, 10, 0, 2, humanic, Coord(51, 9));
	new Enemy(30, 10, 0, 2, humanic, Coord(52, 14));
	new Enemy(30, 10, 0, 2, humanic, Coord(51, 16));
	new Enemy(30, 10, 0, 2, humanic, Coord(51, 24));
	new Enemy(30, 10, 0, 2, humanic, Coord(57, 25));
	new Enemy(30, 10, 0, 2, humanic, Coord(61, 24));
	new Enemy(30, 10, 0, 2, humanic, Coord(69, 22));
	new Enemy(30, 10, 0, 2, humanic, Coord(69, 24));
	new Enemy(30, 10, 0, 2, humanic, Coord(58, 8));
	new Enemy(30, 10, 0, 2, humanic, Coord(58, 11));
	new Enemy(30, 10, 0, 2, humanic, Coord(58, 14));
	new Enemy(30, 10, 0, 2, humanic, Coord(58, 17));
	new Enemy(30, 10, 0, 2, humanic, Coord(58, 20));
	new Enemy(30, 10, 0, 2, humanic, Coord(71, 2));
	new Enemy(30, 10, 0, 2, humanic, Coord(75, 7));
	new Enemy(30, 10, 0, 2, humanic, Coord(75, 17));
	new Enemy(30, 10, 0, 2, humanic, Coord(74, 13));
	new Enemy(30, 10, 0, 2, humanic, Coord(74, 22));

	////ICY REGION
	//
	new Enemy(30, 10, 0, 2, icy, Coord(30, 29));
	new Enemy(30, 10, 0, 2, icy, Coord(28, 33));
	new Enemy(30, 10, 0, 2, icy, Coord(30, 37));
	new Enemy(30, 10, 0, 2, icy, Coord(29, 41));
	new Enemy(30, 10, 0, 2, icy, Coord(30, 47));
	new Enemy(30, 10, 0, 2, icy, Coord(37, 46));
	new Enemy(30, 10, 0, 2, icy, Coord(28, 53));
	new Enemy(30, 10, 0, 2, icy, Coord(28, 57));
	new Enemy(30, 10, 0, 2, icy, Coord(32, 58));
	new Enemy(30, 10, 0, 2, icy, Coord(35, 58));
	new Enemy(30, 10, 0, 2, icy, Coord(38, 58));
	new Enemy(30, 10, 0, 2, icy, Coord(36, 60));
	new Enemy(30, 10, 0, 2, icy, Coord(37, 60));

	////DEMON REGION

	new Enemy(30, 10, 0, 2, demonic, Coord(29, 62));
	new Enemy(30, 10, 0, 2, demonic, Coord(29, 64));
	new Enemy(30, 10, 0, 2, demonic, Coord(34, 71));
	new Enemy(30, 10, 0, 2, demonic, Coord(34, 67));
	new Enemy(30, 10, 0, 2, demonic, Coord(39, 69));
	new Enemy(30, 10, 0, 2, demonic, Coord(47, 69));
	new Enemy(30, 10, 0, 2, demonic, Coord(55, 69));
	new Enemy(30, 10, 0, 2, demonic, Coord(55, 72));
	new Enemy(30, 10, 0, 2, demonic, Coord(47, 72));
	new Enemy(30, 10, 0, 2, demonic, Coord(39, 72));
	new Enemy(30, 10, 0, 2, demonic, Coord(46, 70));
	new Enemy(30, 10, 0, 2, demonic, Coord(48, 70));
	new Enemy(30, 10, 0, 2, demonic, Coord(40, 65));
	new Enemy(30, 10, 0, 2, demonic, Coord(47, 65));
	new Enemy(30, 10, 0, 2, demonic, Coord(54, 65));
	new Enemy(30, 10, 0, 2, demonic, Coord(60, 67));
	new Enemy(30, 10, 0, 2, demonic, Coord(60, 71));
	new Enemy(30, 10, 0, 2, demonic, Coord(47, 51));
	new Enemy(30, 10, 0, 2, demonic, Coord(47, 55));
	new Enemy(30, 10, 0, 2, demonic, Coord(47, 59));
	new Enemy(30, 10, 0, 2, demonic, Coord(47, 62));
	new Enemy(30, 10, 0, 2, demonic, Coord(43, 59));
	new Enemy(30, 10, 0, 2, demonic, Coord(42, 62));
	new Enemy(30, 10, 0, 2, demonic, Coord(52, 62));
	new Enemy(30, 10, 0, 2, demonic, Coord(51, 59));
	new Enemy(30, 10, 0, 2, demonic, Coord(41, 48));
	new Enemy(30, 10, 0, 2, demonic, Coord(42, 48));
	new Enemy(30, 10, 0, 2, demonic, Coord(52, 48));
	new Enemy(30, 10, 0, 2, demonic, Coord(53, 48));
	new Enemy(30, 10, 0, 2, demonic, Coord(44, 50));
	new Enemy(30, 10, 0, 2, demonic, Coord(50, 50));
	new Enemy(30, 10, 0, 2, demonic, Coord(45, 53));
	new Enemy(30, 10, 0, 2, demonic, Coord(49, 53));
	new Enemy(30, 10, 0, 2, demonic, Coord(44, 56));
	new Enemy(30, 10, 0, 2, demonic, Coord(50, 56));

	for (auto i = Enemy::getEnemys().begin(); i != Enemy::getEnemys().end(); i++)
	{
		int x = (*i)->getCoord().x;
		int y = (*i)->getCoord().y;
		{
			enemys[x][y] = 'e';
		}
	}
}

inline void savePlayerAndMap(std::string dir)
{
	std::ofstream fout;
	dir.append("map.dngn");
	fout.open(dir, std::ofstream::binary | std::ofstream::in);
	if (!fout.is_open())
	{
		fout.open(dir, std::ofstream::binary);
		fout.close();
		fout.open(dir, std::ofstream::binary | std::ofstream::in);
	}
	Map::player.saveToFile(fout);
	fout.write((char*)Map::map, sizeof(char) * Map::MAP_SIZE * Map::MAP_SIZE);
	fout.close();
}

inline void saveEnemys(std::string dir)
{
	std::ofstream fout;
	dir.append("enemys.dngn");
	fout.open(dir, std::ofstream::binary | std::ofstream::in);
	if (!fout.is_open())
	{
		fout.open(dir, std::ofstream::binary);
		fout.close();
		fout.open(dir, std::ofstream::binary | std::ofstream::in);
	}
	Enemy::saveToFile(fout);
	fout.close();
}

inline void saveEnvironment(std::string dir)
{
	std::ofstream fout;
	dir.append("environment.dngn");
	fout.open(dir, std::ofstream::binary | std::ofstream::in);
	if (!fout.is_open())
	{
		fout.open(dir, std::ofstream::binary);
		fout.close();
		fout.open(dir, std::ofstream::binary | std::ofstream::in);
	}
	EnvironmentContainer::saveToFile(fout);
	fout.close();
}

inline bool loadPlayerAndMap(std::string dir)
{
	std::ifstream fin;
	dir.append("map.dngn");
	fin.open(dir, std::ifstream::binary | std::ifstream::in);
	if (!fin.is_open())
		return false;
	Map::player.readFromFile(fin);
	fin.read((char*)Map::map, sizeof(char) * Map::MAP_SIZE * Map::MAP_SIZE);
	fin.close();
	return true;
}

inline bool loadEnemys(std::string dir)
{
	std::ifstream fin;
	dir.append("enemys.dngn");
	fin.open(dir, std::ifstream::binary | std::ifstream::in);
	if (!fin.is_open())
		return false;
	Enemy::loadFromFile(fin);
	fin.close();
	return true;
}

inline bool loadEnvironment(std::string dir)
{
	std::ifstream fin;
	dir.append("environment.dngn");
	fin.open(dir, std::ifstream::binary | std::ifstream::in);
	if (!fin.is_open())
		return false;
	EnvironmentContainer::loadFromFile(fin);
	fin.close();
	return true;
}

inline void saveDefaultMap()
{
	createMap();
	std::string defaultPath = "bin/default/";
	savePlayerAndMap(defaultPath);
	saveEnemys(defaultPath);
	saveEnvironment(defaultPath);
}

inline void saveMap()
{
	std::string defaultPath = "bin/save/";
	savePlayerAndMap(defaultPath);
	saveEnemys(defaultPath);
	saveEnvironment(defaultPath);
}

inline bool load()
{
	std::string defaultPath = "bin/save/";
	bool isLoading = true;
	isLoading = loadPlayerAndMap(defaultPath);
	if(isLoading) isLoading = loadEnemys(defaultPath);
	if(isLoading) isLoading = loadEnvironment(defaultPath);
	if (!isLoading)
	{
		EnvironmentContainer::clearMap();
		Enemy::clearEnemys();
		return false;
	}
	for (auto i = Enemy::getEnemys().begin(); i != Enemy::getEnemys().end(); i++)
	{
		int x = (*i)->getCoord().x;
		int y = (*i)->getCoord().y;
		{
			Map::enemys[x][y] = 'e';
		}
	}
	return true;
}

inline void loadDefaultMap()
{
	std::string defaultPath = "bin/default/";
	loadPlayerAndMap(defaultPath);
	loadEnemys(defaultPath);
	loadEnvironment(defaultPath);
	for (auto i = Enemy::getEnemys().begin(); i != Enemy::getEnemys().end(); i++)
	{
		int x = (*i)->getCoord().x;
		int y = (*i)->getCoord().y;
		{
			Map::enemys[x][y] = 'e';
		}
	}
}