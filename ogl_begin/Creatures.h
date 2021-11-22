#pragma once

#include "Items.h"
#include <iostream>

#define WEAPON 0
#define HELMET 1
#define BIB 2
#define PANTS 3
#define BOOTS 4


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
		int maxHp;
		int damage;
		int protection;
		int exp;
		int movepoints;
		CreatureType type;
		Coord coord;

	public:
		inline Creature() : hp(100), maxHp(hp), damage(30), protection(0), exp(1), movepoints(1), type(humanic), coord(Coord(0, 0)) { }
		inline Creature(int hp, int dmg, int pro, int exp, CreatureType t, Coord spawnCoord) : hp(hp), maxHp(hp), movepoints(1), damage(dmg), protection(pro), exp(exp), type(t), coord(spawnCoord) { }
		virtual inline ~Creature() { }

		inline void setExp(int exp) { this->exp = exp; }
		inline void setCoord(Coord coord) { this->coord = coord; }
		inline void setHp(int hp) { this->hp = hp; }
		inline void setMaxHp(int hp) { maxHp = hp; }
		inline int getInventorySize() const { return INVENTORY_SIZE; }
		inline int getHp() const { return hp; }
		inline int getMaxHp() const { return maxHp; }
		inline int getDamage() const { return damage; }
		inline int getProtection() const { return protection; }
		inline int getExp() const { return exp; }
		inline int getMovepoints() const { return movepoints; }
		inline CreatureType getType() const { return type; }
		inline Coord getCoord() const { return coord; }
		virtual void move(Direction vector, const int& mapSize);
		ItemMassive* hit(Creature& creature);
		virtual void setMovepoints();
		virtual inline void setMovepoints(int mp) { movepoints = mp; }
	};

	class Player : public Creature //x, y - Относительно карты, а не массива символов.
	{
	private:
		static const int INVENTORY_SIZE = 8;
		int lvl;
		int lvlPoints;
		int agility;
		int intelligence;
		int power;
		Item** inventory;
		Equipment** equipment;
		int items;
	public:
		inline Player(int hp, int dmg, int pro, int exp, CreatureType t, Coord spawnCoord) : Creature(hp, dmg, pro, exp, t, spawnCoord), items(0)
		{
			inventory = new Item * [INVENTORY_SIZE];
			equipment = new Equipment * [5];
			for (int i = 0; i < INVENTORY_SIZE; i++) inventory[i] = nullptr;
			for (int i = 0; i < 5; i++) equipment[i] = nullptr;
			lvl = 1;
			lvlPoints = 0;
			agility = 1;
			intelligence = 1;
			power = 1;
		}
		inline ~Player() { delete[] inventory; delete[] equipment; }
		inline Item** const& getInventory() const { return inventory; }
		inline Equipment** const& getEquipment() const { return equipment; }
		inline void setItems(int items) { this->items = items; }
		inline int getItems() const { return items; }
		inline int getAgility() const { return agility; }
		inline void setAgility(int a) { agility = a; }
		static inline int getInventorySize() { return INVENTORY_SIZE; }

		void pickup(Item* item);
		void upLvl();
		void upStats(Stat stat);
		inline void setMovepoints(int mp) override { Creature::setMovepoints(mp); }
		void setMovepoints() override;
		void equip(int index);
		void dropItem(int index);
		void move(Direction vector, const int& mapSize) override;
	};

	class Enemy : public Creature
	{
	private:

	public:
		inline Enemy(int hp, int dmg, int pro, int exp, CreatureType t, Coord spawnCoord) : Creature(hp, dmg, pro, exp, t, spawnCoord)
		{

		}
	};
