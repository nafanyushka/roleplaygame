#pragma once

#include "Items.h"
#include <iostream>

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

struct Coord
{
	int x, y;
	inline Coord(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	//inline friend bool operator < (const Coord coord1, const Coord coord2) { return coord1.x + coord1.y < coord2.x + coord2.y; }
	inline bool operator < (const Coord& c) const noexcept { return this->x + this->y < c.x + c.y; }
};

struct ItemMassive
{
	Item* massive;
	int size;
};

class Creature
{
private:
	static const int INVENTORY_SIZE = 10;
	int hp;
	int damage;
	int protection;
	int exp;
	int movepoints;
	CreatureType type;
	Coord coord;
	
public:
	inline Creature() : hp(100), damage(30), protection(0), exp(1), movepoints(1), type(humanic), coord(Coord(0, 0)) { }
	inline Creature(int hp, int dmg, int pro, int exp, CreatureType t, Coord spawnCoord) : hp(hp),  movepoints(1), damage(dmg), protection(pro), exp(exp), type(t), coord(spawnCoord) { }
	virtual inline ~Creature() { }
	
	inline void setExp(int exp) { this->exp = exp; }
	inline void setCoord(Coord coord) { this->coord = coord; }
	inline int getInventorySize() const { return INVENTORY_SIZE; }
	inline int getHp() const { return hp; }
	inline int getDamage() const { return damage; }
	inline int getProtection() const { return protection; }
	inline int getExp() const { return exp; }
	inline int getMovepoints() const { return movepoints; }
	inline CreatureType getType() const { return type; }
	inline Coord getCoord() const { return coord; }
	void move(Direction vector);
	ItemMassive* hit(Creature& creature);
	virtual void setMovepoints();
	inline void setMovepoints(int mp) { movepoints = mp; }
};

class Player : public Creature //x, y - Относительно карты, а не массива символов.
{
private:
	int lvl;
	int lvlPoints;
	int agility;
	int intelligence;
	int power;
	Item* weapon;
	Item* inventory;
	int items;
public:
	inline Player(int hp, int dmg, int pro, int exp, CreatureType t, Coord spawnCoord) : Creature(hp, dmg, pro, exp, t, spawnCoord), items(0)
	{
		inventory = new Item[getInventorySize()];
		lvl = 1;
		weapon = nullptr;
		lvlPoints = 0;
		agility = 1;
		intelligence = 1;
		power = 1;
	}
	inline ~Player() { delete[] inventory; }
	inline Item* getInventory() const { return inventory; }
	inline void setItems(int items) { this->items = items; }
	inline int getItems() const { return items; }
	inline int getAgility() { return agility; }

	void pickup(const Item& item);
	void upLvl();
	void upStats(Stat stat);
	inline void setMovepoints(int mp) { setMovepoints(mp); }
	void setMovepoints() override;
	void setWeapon(Item* weapon);
	void dropItem(int index);
};

class Enemy : public Creature
{
private:

public:
	inline Enemy(int hp, int dmg, int pro, int exp, CreatureType t, Coord spawnCoord) : Creature(hp, dmg, pro, exp, t, spawnCoord)
	{

	}
};