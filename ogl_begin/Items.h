#pragma once

#include "Creatures.h"

#define AGIL 0
#define POW 1
#define INT 2
#define DMG 3

enum ItemType
{
	weapon,
	protection,
	potion,
	masterkey
};

class Item
{
private:
	ItemType type;
	bool isArt = false;
public:
	inline Item() { type = masterkey; };	inline Item(const ItemType type) : type(type) {}
	inline ItemType getType() { return type; }
};

class Art : public Item
{
private:
	bool isArt = true;
	int dCharacteristic[4]; //»«Ã≈Õ≈Õ»≈ ’¿–¿ “≈–»—“» 
public:
	
};