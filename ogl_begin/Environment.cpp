#include "Environment.h"
#include <ctime>
	std::map<std::pair<int, int>, Environment*> Environment::environmentOnMap;

	void Environment::clearMap()
	{
		for (std::map<std::pair<int, int>, Environment*>::iterator it = environmentOnMap.begin(); it != environmentOnMap.end(); it++)
		{
			Environment* point = it->second;
			delete point;
		}
	}

	void Chest::iterate(Player& player)
	{
		if (item == nullptr)
			return;
		if (getStatus())
		{
			for (int i = 0; i < player.getItems(); i++)
			{
				if (player.getInventory()[i]->getType() == ItemType::masterkey)
				{
					float chance = lvl == 0 ? 100 : 100.0f / (float)lvl;
					float personalChance = (int)((float)(rand() % 100) / (float)player.getAgility());
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