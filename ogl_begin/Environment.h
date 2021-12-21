#pragma once
#include "Creatures.h"
#include "Map.h"
#include "Items.h"
#include <map>
#include <fstream>


	class Environment
	{
	private:
		Coord coord;
		bool status = false;
	public:
		virtual inline ~Environment()
		{
			//delete environmentOnMap;
		}
		inline Environment(Coord c, bool status) : coord(c), status(status) { /*environmentOnMap = new std::map<std::pair<int, int>, Environment>;*/ }
		inline bool getStatus() const { return status; }
		inline void setStatus(bool status) { this->status = status; }
		inline Coord* getCoord() { return &coord; }
		virtual inline void iterate(Player& player) = 0;
		static Environment* getEnvironment(int x, int y);
		virtual std::string getString();
		virtual void save(std::ofstream& fout) = 0;
		virtual void load(std::ifstream& fin) = 0;
	};

	class EnvironmentContainer
	{
	private:
		static std::map<std::pair<int, int>, Environment*> environmentOnMap; //–¿«ƒ≈À»“‹ Õ¿ ENVIRONMENT Ë ELEMENT OF ENVIRONMENT
	public:
		inline static std::map<std::pair<int, int>, Environment*>& getMap() { return environmentOnMap; }

		static void saveToFile(std::ofstream& fout);
		static void loadFromFile(std::ifstream& fin);
		static void clearMap();
	};

	class Chest : public Environment
	{
	private:
		Item* item;
		int lvl = 1;
	public:
		void save(std::ofstream& fout) override;
		void load(std::ifstream& fin) override;
		inline Chest() : Environment(Coord(0,0), false), item(nullptr), lvl(0) {}
		inline Chest(Coord coord, Item* item, bool isClosed, int lvl) : Environment(coord, isClosed), item(item), lvl(lvl) { EnvironmentContainer::getMap().emplace(std::pair<int, int>(coord.x, coord.y), this); }
		inline ~Chest() { if (item != nullptr) delete item; }
		void iterate(Player& player) override;
		std::string getString() override;
	};



	class Door : public Environment
	{
	private:
		int lvl = 1;
	public:
		inline Door() : Environment(Coord(0, 0), false), lvl(0) {}
		inline Door(Coord coord, bool isClosed, int lvl) : Environment(coord, isClosed), lvl(lvl) { EnvironmentContainer::getMap().emplace(std::pair<int, int>(coord.x, coord.y), this); }
		virtual inline ~Door() {}
	
		void save(std::ofstream& fout) override;
		void load(std::ifstream& fin) override;
		void iterate(Player& player) override;
		std::string getString() override;
	};

	class ExitDoor : public Door
	{
	public:
		inline ExitDoor(int x, int y) : Door(Coord(x, y), true, 4) {}
		inline ExitDoor() : Door() {}
		inline void iterate(Player& player) override
		{
			Door::iterate(player);
			if(!getStatus())
				player.getIsEnd() = true;
		}
		void load(std::ifstream& fin) override;
		void save(std::ofstream& fout) override;
		inline ExitDoor(Coord& coord) : Door(coord, true, 20) {}
	};

	