#pragma once
#include <iostream>
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

namespace Map
{
	const int VISION = 8;
	const int MAP_SIZE = 10;
}