#pragma once
#include <iostream>
#include <fstream>
//#include "Creatures.h"
//#include "Environment.h"
//#include "Items.h"
//
//int getCoordPlane(int coord); int getCoordMap(int coord);
//

enum Direction
{
	top,
	bottom,
	left,
	right
};

enum CreatureType
{
	zombie,
	humanic,
	zoo,
	demonic,
	icy,
};

enum Stat
{
	agil,
	power,
	intel
};

enum ItemType
{
	equipment,
	potion,
	masterkey
};

enum EquipmentType
{
	weapon,
	helmet,
	bib,
	pants,
	boots
};

enum EffectType
{
	hp,
	mana,
	dmg
};

enum LoadingItemType
{
	mstrk,
	ptn,
	wpn,
	prt,
	aprt,
	awpn,
	ewpn,
	aewpn
};

enum LoadingEnvironmentType
{
	chest,
	door,
	exitdoor
};

enum Scene
{
	menu,
	game,
	ext,
	win,
	lose
};

namespace Map
{
	const int VISION = 8;
	const int MAP_SIZE = 100;
}