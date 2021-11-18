#pragma once
#include "Creatures.h"
#include "Items.h"
#include <map>

class Environment
{
private:
	static std::map<std::pair<int, int>, Environment*> environmentOnMap;
	Coord coord;
	bool status = false;
public:
	virtual inline ~Environment()
	{
		//delete environmentOnMap;
	}
	inline Environment(Coord& c, bool status) : coord(c), status(status) { /*environmentOnMap = new std::map<std::pair<int, int>, Environment>;*/ }

	inline bool getStatus() const { return status; }
	inline void setStatus(bool status) { this->status = status; }
	static inline std::map<std::pair<int, int>, Environment*>* getMap() { return &environmentOnMap; }
	inline Coord* getCoord() { return &coord; }
	virtual inline void iterate(Player& player) {}
};

class Chest : public Environment
{
private:
	Item* item;
	int lvl = 1;
public:
	inline Chest(Coord& coord, Item& item, bool isClosed, int lvl) : Environment(coord, isClosed), item(&item), lvl(lvl) { }
	inline ~Chest() { if (item != nullptr) delete item; }
	void iterate(Player& player) override;
};

class Door : public Environment
{
private:
	int lvl = 1;
public:
	inline Door(Coord& coord, bool isClosed, int lvl) : Environment(coord, isClosed), lvl(lvl) { }
	inline ~Door() {}
	
	void iterate(Player& player) override;
};