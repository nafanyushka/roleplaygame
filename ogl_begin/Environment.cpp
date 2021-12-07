#include "Environment.h"
#include <ctime>
#include <string>
	std::map<std::pair<int, int>, Environment*> Environment::environmentOnMap;

	void Environment::clearMap()
	{
		for (std::map<std::pair<int, int>, Environment*>::iterator it = environmentOnMap.begin(); it != environmentOnMap.end(); it++)
		{
			Environment* point = it->second;
			delete point;
		}
	}

	Environment* Environment::getEnvironment(int x, int y)
	{
		return Environment::getMap()->find(std::pair<int, int>(x, y)) == Environment::getMap()->end() ? nullptr : Environment::getMap()->find(std::pair<int, int>(x, y))->second;
	}

	std::string Environment::getString()
	{
		std::string string = "Environment";
		return string;
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
					float personalChance = (int)((float)(rand() % 100) / ((float)player.getAgility() / 3.0f));
					if (personalChance <= (int)chance) {
						//player.dropItem(i);
						setStatus(false);
						player.setMovepoints(player.getMovepoints() - 1);
						player.pickup(item);
						item = nullptr;
						return;
					}
					else
					{
						player.setMovepoints(player.getMovepoints() - 1);
						player.dropItem(i);
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
						return;
					}
					else
					{
						player.dropItem(i);
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
