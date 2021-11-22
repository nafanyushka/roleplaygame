#include "Creatures.h"
#include <math.h>

void Creature::move(Direction vector, const int& mapSize)
	{
		switch (vector)
		{
		case top:
			if (coord.y < mapSize - 1)
			{
				coord.y++;
			}
			else return;
			break;
		case bottom:
			if (coord.y > 0)
			{
				coord.y--;
			}
			else return;
			break;
		case right:
			if (coord.x < mapSize - 1)
			{
				coord.x++;
			}
			else return;
			break;
		case left:
			if (coord.x > 0)
			{
				coord.x--;
			}
			else return;
			break;
		}
		movepoints--;
	}

ItemMassive* Creature::hit(Creature& creature)
	{
		creature.hp -= damage;
		movepoints--;
		ItemMassive* im = nullptr;
		if (creature.hp <= 0)
		{
			this->exp += creature.exp;
		}
		return im;
	}

void Creature::setMovepoints()
	{
		switch (type)
		{
		case zombie:
			if (movepoints == -1)
				movepoints = 1;
			else if (movepoints == 0)
				movepoints = -1;
			break;
		case humanic:
			movepoints++;
			break;
		case demonic:
			movepoints += 2;
			break;
		case zoo:
			movepoints += 3;
			break;
		case icy:
			if (movepoints == -1)
				movepoints = 1;
			else if (movepoints == 0)
				movepoints = -1;
			break;
		}
	}

void Player::pickup(Item* item)
	{
		if (getItems() >= 10)
			return;
		(getInventory()[getItems()]) = item;
		setItems(getItems() + 1);
	}

void Player::upLvl()
	{
		if (getExp() < (int)pow(2, lvl))
			return;
		lvlPoints += ++lvl;
		setExp(getExp() - (int)pow(2, lvl));
	}

void Player::upStats(Stat stat)
	{
		if (lvlPoints == 0)
			return;
		switch (stat)
		{
		case agil:
			agility++;
			break;
		case Stat::power:
			power++;
			break;
		case intel:
			intelligence++;
			break;
		}
		lvlPoints--;
	}

void Player::setMovepoints()
	{
		switch (getType())
		{
		case zombie:
			if (getMovepoints() == -1)
				setMovepoints(1);
			else if (getMovepoints() == 0)
				setMovepoints(-1);
			break;
		case humanic:
			setMovepoints(1);
			break;
		case demonic:
			setMovepoints(2);
			break;
		case zoo:
			setMovepoints(3);
			break;
		case icy:
			if (getMovepoints() == -1)
				setMovepoints(1);
			else if (getMovepoints() == 0)
				setMovepoints(-1);
			break;
		}
		setMovepoints(getMovepoints() + agility);
	}

void Player::equip(int index)
	{
		if (index >= getItems())
			return;
		if (getInventory()[index]->getType() != ItemType::equipment)
			return;
		Equipment* eq = dynamic_cast<Equipment*>(getInventory()[index]);
		int select;
		switch (eq->getEquipmentType())
		{
		case weapon:
			select = WEAPON;
			break;
		case helmet:
			select = HELMET;
			break;
		case pants:
			select = PANTS;
			break;
		case bib:
			select = BIB;
			break;
		case boots:
			select = BOOTS;
			break;
		}
		if (equipment[select] != nullptr)
		{
			getInventory()[index] = dynamic_cast<Item*>(equipment[select]);
			equipment[select] = eq;
		}
		else
		{
			equipment[select] = eq;
			getInventory()[index] = getInventory()[--items];
		}
	}

void Player::dropItem(int index)
	{
		if (index > getItems() || index < 0)
			return;
		getInventory()[index] = getInventory()[getItems() - 1];
		setItems(getItems() - 1);
	}

void Player::move(Direction vector, const int& mapSize)
	{
		switch (vector)
		{
		case top:
			setCoord(Coord(getCoord().x, getCoord().y + 1));
			break;
		case bottom:
			setCoord(Coord(getCoord().x, getCoord().y - 1));
			break;
		case right:
			setCoord(Coord(getCoord().x + 1, getCoord().y));
			break;
		case left:
			setCoord(Coord(getCoord().x - 1, getCoord().y));
			break;
		}
		setMovepoints(getMovepoints() - 1);
	}

	//void main()
	//{
	//	Player player(100, 30, 4, 0, humanic, Coord(0, 0));
	//	Creature zombie;
	//	player.hit(zombie);
	//	std::cout << zombie.getHp() << std::endl;
	//}
