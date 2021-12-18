#pragma once

#include "Map.h"
#include "Items.h"
#include <iostream>
#include <fstream>
#include <irrKlang.h>
//#include <list>
#include "library/List.h"
using namespace irrklang;

#define WEAPON 0
#define HELMET 1
#define BIB 2
#define PANTS 3
#define BOOTS 4
#define EQUIPMENT_SIZE 5
	
int getCoordPlane(int coord); int getCoordMap(int coord);

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
		inline static int getInventorySize() { return INVENTORY_SIZE; }
		inline int getHp() const { return hp; }
		inline int getMaxHp() const { return maxHp; }
		virtual inline int getDamage() const { return damage; }
		virtual inline int getProtection() const { return protection; }
		inline int getExp() const { return exp; }
		inline int getMovepoints() const { return movepoints; }
		inline CreatureType getType() const { return type; }
		inline Coord getCoord() const { return coord; }
		inline void setDamage(int dmg) { damage = dmg; }
		virtual void move(Direction vector, const int& mapSize);
		virtual ItemMassive* hit(Creature& creature);
		virtual void setMovepoints();
		virtual inline void setMovepoints(int mp) { movepoints = mp; }
		virtual std::string getString();
	};

	class Player : public Creature //x, y - Îòíîñèòåëüíî êàğòû, à íå ìàññèâà ñèìâîëîâ.
	{
	private:
		ISoundEngine* playerSound = nullptr; // ÍÅ ÇÀÁÛÂÀÅÌ ÅÃÎ ÈÍÈÖÈÀËÈÇÈĞÎÂÀÒÜ!
		static const int INVENTORY_SIZE = 8;
		int lvl;
		int lvlPoints;
		int agility;
		int intelligence;
		int power;
		Item** inventory;
		Equipment** equipment;
		int items;
		bool isEnd = false;

		int getDamageTo(CreatureType target) const;
	public:
		inline Player(int hp, int dmg, int pro, int exp, CreatureType t, Coord spawnCoord) : Creature(hp, dmg, pro, exp, t, spawnCoord), items(0)
		{
			inventory = new Item * [INVENTORY_SIZE];
			equipment = new Equipment * [EQUIPMENT_SIZE];
			for (int i = 0; i < INVENTORY_SIZE; i++) inventory[i] = nullptr;
			for (int i = 0; i < 5; i++) equipment[i] = nullptr;
			lvl = 1;
			lvlPoints = 0;
			agility = 1;
			intelligence = 1;
			power = 1;
		}
		inline ~Player() 
		{
			if (inventory != nullptr)
			{
				for (int i = 0; i < getItems(); i++)
				{
					delete inventory[i];
				}
				delete[] inventory;
			}
			if (equipment != nullptr)
			{
				for (int i = 0; i < EQUIPMENT_SIZE; i++)
				{
					if (equipment[i] != nullptr) delete equipment[i];
				}
				delete[] equipment;
			}
			if (playerSound != nullptr) playerSound->drop();
		}
		inline ISoundEngine*& getPlayerSound() { return playerSound; }
		inline bool& getIsEnd() { return isEnd; }
		inline Item** const& getInventory() const { return inventory; }
		inline Equipment** const& getEquipment() const { return equipment; }
		inline void setItems(int items) { this->items = items; }
		inline int getItems() const { return items; }
		int getAgility() const;
		int getProtection() const override;
		int getPower() const;
		int getIntelligence() const;
		int getDamage() const override;
		inline void heal(int hp) { if (this->getHp() + hp > this->getMaxHp()) setHp(getMaxHp()); else setHp(getHp() + hp); };
		inline void setAgility(int a) { agility = a; }
		static inline int getInventorySize() { return INVENTORY_SIZE; }
		void saveToFile(std::ofstream& fout);
		void readFromFile(std::ifstream& fin);

		std::string getLvlString();
		std::string getAgilityString();
		std::string getPowerString();
		std::string getIntelligenceString();
		std::string getProtectionString();
		std::string getDmgString();
		std::string getMovepointsString();
		std::string getExpString();
		void pickup(Item* item);
		void upLvl();
		void upStats(Stat stat);
		inline void setMovepoints(int mp) override { Creature::setMovepoints(mp); }
		void setMovepoints() override;
		void equip(int index);
		void unequip(int index);
		void dropItem(int index);
		void move(Direction vector, const int& mapSize) override;
		ItemMassive* hit(Creature& creature) override;
	};

	class Enemy : public Creature
	{
	private:
		static std::list<Enemy*> enemys;
	public:
		inline Enemy() : Creature(100, 1, 1, 1, zombie, Coord(0, 0)) {}
		inline Enemy(int hp, int dmg, int pro, int exp, CreatureType t, Coord spawnCoord) : Creature(hp, dmg, pro, exp, t, spawnCoord)
		{
			enemys.push_back(this);
		}
		inline ~Enemy() {}
		Direction* goToPlayer(Player& player, const int& mapSize, char( *map )[Map::MAP_SIZE], char ( *enemyMap )[Map::MAP_SIZE], int& pathSize);
		static inline std::list<Enemy*>& getEnemys() { return enemys; }
		static void clearEnemys();
		static void saveToFile(std::ofstream& fout);
		static void loadFromFile(std::ifstream& fin);
		static Enemy* getEnemy(int x, int y);
	};
