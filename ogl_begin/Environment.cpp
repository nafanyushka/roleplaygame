#include "Environment.h"
#include <ctime>

std::map<std::pair<int, int>, Environment*> Environment::environmentOnMap;

void Chest::iterate(Player& player)
{
	if (item == nullptr)
		return;
	if (getStatus())
	{
		for (int i = 0; i < player.getItems(); i++)
		{
			if (player.getInventory()[i].getType() == ItemType::masterkey)
			{
				float chance = lvl == 0 ? 100 : 100.0f / (float)lvl;
				if ((int)((float)(rand() % 100) / (float)player.getAgility()) <= (int)chance) {
					player.dropItem(i);
					setStatus(false);
				}
				else
				{
					player.dropItem(i);
					return;
				}
				break;
			}
			if(i + 1 == player.getItems()) return;
		}
	}
	player.pickup(*item);
	item = nullptr;
}

void Door::iterate(Player& player)
{
	if (getStatus())
	{
		for (int i = 0; i < player.getItems(); i++)
		{
			if (player.getInventory()[i].getType() == ItemType::masterkey)
			{
				float chance = lvl == 0 ? 100 : 100.0f / (float)lvl;
				if ((int)((float)(rand() % 101) / (float)player.getAgility()) <= (int)chance) {
					player.dropItem(i);
					setStatus(false);
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
