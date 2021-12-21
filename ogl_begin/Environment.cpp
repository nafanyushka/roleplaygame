#include "Environment.h"
#include <ctime>
#include <string>
	std::map<std::pair<int, int>, Environment*> EnvironmentContainer::environmentOnMap;

	void EnvironmentContainer::clearMap()
	{
		for (std::map<std::pair<int, int>, Environment*>::iterator it = environmentOnMap.begin(); it != environmentOnMap.end(); it++)
		{
			Environment* point = it->second;
			delete point;
		}
	}

	Environment* Environment::getEnvironment(int x, int y)
	{
		return EnvironmentContainer::getMap().find(std::pair<int, int>(x, y)) == EnvironmentContainer::getMap().end() ? nullptr : EnvironmentContainer::getMap().find(std::pair<int, int>(x, y))->second;
	}

	std::string Environment::getString()
	{
		std::string string = "Environment";
		return string;
	}

	void EnvironmentContainer::saveToFile(std::ofstream& fout)
	{
		int size = environmentOnMap.size();
		fout.write((char*)&size, sizeof(int));
		int count = 0;
		for (auto i = environmentOnMap.begin(); i != environmentOnMap.end(); i++)
		{
			if (count == 31)
				std::cout << "BRO" << std::endl;
			(*i).second->save(fout);
			count++;
		}
	}

	void EnvironmentContainer::loadFromFile(std::ifstream& fin)
	{
		int size = 0;
		fin.read((char*)&size, sizeof(int));
		Environment* e = nullptr;
		for (int i = 0; i < size; i++)
		{
			LoadingEnvironmentType let = LoadingEnvironmentType::exitdoor;
			fin.read((char*)&let, sizeof(int));
			switch (let)
			{
			case LoadingEnvironmentType::chest:
				e = new Chest();
				break;
			case door:
				e = new Door();
				break;
			case exitdoor:
				e = new ExitDoor();
				break;
			}
			if (i == 31)
				std::cout << "BAN!" << std::endl;
			e->load(fin);
			environmentOnMap.emplace(std::pair<int, int>(e->getCoord()->x, e->getCoord()->y), e);
		}
	}

	void Chest::iterate(Player& player)
	{
		if (item == nullptr)
			return;
		if (player.getItems() >= Creature::getInventorySize())
			return;
		if (getStatus())
		{
			for (int i = 0; i < player.getItems(); i++)
			{
				if (player.getInventory()[i]->getType() == ItemType::masterkey)
				{
					float chance = lvl == 0 ? 100 : 100.0f / (float)lvl;
					float personalChance = (int)((float)(rand() % 100) / ((float)player.getAgility() / 1.5f));
					if (personalChance <= (int)chance) {
						//player.dropItem(i);
						setStatus(false);
						if (player.getPlayerSound() != nullptr)
							player.getPlayerSound()->play2D("sound\\open.wav", false);
						player.setMovepoints(player.getMovepoints() - 1);
						player.pickup(item);
						if (player.getPlayerSound() != nullptr)
							player.getPlayerSound()->play2D("sound\\equip.wav", false);
						item = nullptr;
						return;
					}
					else
					{
						player.setMovepoints(player.getMovepoints() - 1);
						player.dropItem(i);
						if (player.getPlayerSound() != nullptr)
						{
							player.getPlayerSound()->stopAllSounds();
							player.getPlayerSound()->play2D("sound\\crashmasterkey.wav", false);
						}
						return;
					}
					break;
				}
				if (i + 1 == player.getItems()) return;
			}
		}
		else
		{
			player.setMovepoints(player.getMovepoints() - 1);
			player.pickup(item);
			if (player.getPlayerSound() != nullptr)
				player.getPlayerSound()->play2D("sound\\equip.wav", false);
			item = nullptr;
			return;
		}
	}

	std::string Chest::getString()
	{
		std::string string = "Chest: ";
		string.append(std::to_string(lvl));
		string.append("; ");
		string.append(getStatus() == false ? "opened; " : "closed; ");
		string.append(item == nullptr ? "empty." : "item inside.");
		return string;
	}

	void Chest::load(std::ifstream& fin)
	{
		fin.read((char*)this + sizeof(void*), sizeof(Chest) - sizeof(void*));
		if (item != nullptr)
		{
			item = loadItem(fin);
		}
	}

	void Door::iterate(Player& player)
	{
		if (getStatus())
		{
			for (int i = 0; i < player.getItems(); i++)
			{
				if (player.getInventory()[i]->getType() == ItemType::masterkey)
				{
					float chance = lvl == 0 ? 100 : 100.0f / (float)lvl;
					player.setMovepoints(player.getMovepoints() - 1);
					if ((int)((float)(rand() % 101) / (float)player.getAgility()) <= (int)chance) {
						//player.dropItem(i);
						setStatus(false);
						if (player.getPlayerSound() != nullptr)
							player.getPlayerSound()->play2D("sound\\open.wav", false);
						return;
					}
					else
					{
						player.dropItem(i);
						if (player.getPlayerSound() != nullptr)
						{
							player.getPlayerSound()->stopAllSounds();
							player.getPlayerSound()->play2D("sound\\crashmasterkey.wav", false);
						}
						return;
					}
				}
			}
		}
	}

	std::string Door::getString()
	{
		std::string string = "Door: ";
		string.append(std::to_string(lvl));
		string.append("; ");
		string.append(getStatus() == false ? "opened." : "closed.");
		return string;
	}

	void Chest::save(std::ofstream& fout)
	{
		LoadingEnvironmentType let = chest;
		fout.write((char*)&let, sizeof(int));
		fout.write((char*)this + sizeof(void*),sizeof(Chest) - sizeof(void*));
		if (item != nullptr)
		{
			item->saveToFile(fout);
		}
	}

	void Door::save(std::ofstream& fout)
	{
		LoadingEnvironmentType let = door;
		fout.write((char*)&let, sizeof(int));
		fout.write((char*)this + sizeof(void*),sizeof(Door) - sizeof(void*));
	}

	void Door::load(std::ifstream& fin)
	{
		fin.read((char*)this  + sizeof(void*),sizeof(Door) - sizeof(void*));
	}

	void ExitDoor::save(std::ofstream& fout)
	{
		LoadingEnvironmentType let = exitdoor;
		fout.write((char*)&let, sizeof(int));
		fout.write((char*)this + sizeof(void*),sizeof(ExitDoor) - sizeof(void*));
	}

	void ExitDoor::load(std::ifstream& fin)
	{
		fin.read((char*)this + sizeof(void*),sizeof(ExitDoor) - sizeof(void*));
	}
