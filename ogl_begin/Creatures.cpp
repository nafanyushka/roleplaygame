#include "Creatures.h"
#include "Map.h"
#include <math.h>
#include <string>
#define ZOMBIE_MOVE 5
#define HUMAN_MOVE 3
#define DEMON_MOVE 2
#define ZOO_MOVE 5
#define ICY_MOVE 7

std::list<Enemy*> Enemy::enemys;

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
		int dmg = getDamage() - creature.getProtection();
		creature.hp -= dmg < 0 ? 0 : dmg;
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
				movepoints = ZOMBIE_MOVE;
			else if (movepoints == 0)
				movepoints = -1;
			break;
		case humanic:
			movepoints += HUMAN_MOVE;
			break;
		case demonic:
			movepoints += DEMON_MOVE;
			break;
		case zoo:
			movepoints += ZOO_MOVE;
			break;
		case icy:
			if (movepoints == -1)
				movepoints = ICY_MOVE;
			else if (movepoints == 0)
				movepoints = -1;
			break;
		}
	}

std::string Creature::getString()
{
	std::string str;
	str.append(
		type == zombie ? "Zombie " :
		type == CreatureType::humanic ? "Human " :
		type == CreatureType::demonic ? "Demon " :
		type == CreatureType::icy ? "Icy " : "Zoo"
	);
	str.append("EXP: ");
	str.append(std::to_string(exp));
	str.append(" HP: ");
	str.append(std::to_string(hp));
	str.append(" PROTECTION: ");
	str.append(std::to_string(protection));
	str.append(" DMG: ");
	str.append(std::to_string(damage));
	str.append(".");
	return str;
}

int Player::getDamageTo(CreatureType target) const //ÐÀÑ×ÅÒ ÓÐÎÍÀ Ñ ÎÐÓÆÈß
{
	if (equipment[WEAPON] == nullptr)
		return getDamage();
	Weapon* weapon = dynamic_cast<Weapon*>(equipment[WEAPON]);
	if (!weapon->getIsEnchanted())
		return getDamage();
	EnchantedWeapon* enchantedWeapon = dynamic_cast<EnchantedWeapon*>(weapon);
	return enchantedWeapon->getDmgTo(target) + getDamage();
}

int Player::getAgility() const
{
	int resultAgility = agility;
	for (int i = 0; i < EQUIPMENT_SIZE; i++)
	{
		resultAgility += equipment[i] == nullptr ? 0 : equipment[i]->getAgil();
	}
	return resultAgility;
}

int Player::getProtection() const
{
	int prot = Creature::getProtection();
	for (int i = 0; i < EQUIPMENT_SIZE; i++)
	{
		prot += equipment[i] == nullptr ? 0 : equipment[i]->getProtection();
	}
	return prot;
}

int Player::getPower() const
{
	int resultPower = power;
	for (int i = 0; i < EQUIPMENT_SIZE; i++)
	{
		resultPower += equipment[i] == nullptr ? 0 : equipment[i]->getPow();
	}
	return resultPower;
}

int Player::getIntelligence() const
{
	int resultInt = intelligence;
	for (int i = 0; i < EQUIPMENT_SIZE; i++)
	{
		resultInt += equipment[i] == nullptr ? 0 : equipment[i]->getInt();
	}
	return resultInt;
}

int Player::getDamage() const
{
	int resultDmg = Creature::getDamage();
	for (int i = 0; i < EQUIPMENT_SIZE; i++)
	{
		resultDmg += equipment[i] == nullptr ? 0 : equipment[i]->getDmg();
	}
	return resultDmg;
}

std::string Player::getLvlString()
{
	return std::to_string(lvl);
}

std::string Player::getAgilityString()
{
	return std::to_string(getAgility());
}

std::string Player::getPowerString()
{
	return std::to_string(getPower());
}

std::string Player::getIntelligenceString()
{
	return std::to_string(getIntelligence());
}

std::string Player::getProtectionString()
{
	return std::to_string(getProtection());
}

std::string Player::getDmgString()
{
	return std::to_string(getDamage());
}

std::string Player::getMovepointsString()
{
	return std::to_string(getMovepoints());
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
		setMovepoints(getMovepoints() + getAgility());
	}

void Player::equip(int index)
	{
		if (index >= getItems())
			return;
		ItemType t = getInventory()[index]->getType();
		if (t != ItemType::equipment)
		{
			if(t != potion)
				return;
			Potion* potion = dynamic_cast<Potion*>(getInventory()[index]);
			switch (potion->getEffectType())
			{
			case EffectType::hp:
				heal(potion->getEffect());
				dropItem(index);
				break;
			case mana:
				break;
			case dmg:
				setDamage(getDamage() + potion->getEffect());
				break;
			}
			return;
		}
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

ItemMassive* Player::hit(Creature& creature)
{
	int dmg = getDamageTo(creature.getType());
	dmg = dmg > creature.getProtection() ? dmg - creature.getProtection() : 0;
	creature.setHp(creature.getHp() - dmg);
	setMovepoints(getMovepoints() - 1);
	ItemMassive* im = nullptr;
	if (creature.getHp() <= 0)
	{
		setExp(getExp() + creature.getExp());
	}
	return im;
}

//ItemMassive* Player::hit(Creature& creature)
//{
//	creature.setHp(creature.getHp() - getDamage());
//	setMovepoints(getMovepoints() - 1);
//	return nullptr;
//}

	//void main()
	//{
	//	Player player(100, 30, 4, 0, humanic, Coord(0, 0));
	//	Creature zombie;
	//	player.hit(zombie);
	//	std::cout << zombie.getHp() << std::endl;
	//}
	// 
//--------------------------------------------------BOT LOGIC
char** createMovementMap(const int& mapSize, char (*map)[Map::MAP_SIZE], char (*enemyMap)[Map::MAP_SIZE])
{
	char** movementMap = new char* [mapSize];
	for (int i = 0; i < mapSize; i++)
	{
		movementMap[i] = new char[mapSize];
		for (int j = 0; j < mapSize; j++)
		{
			movementMap[i][j] = enemyMap[i][j] == 'e' ? '#' : map[i][j];
		}
	}
	return movementMap;
}

void deleteMovementMap(char**& movementMap, const int& mapSize)
{
	for (int i = 0; i < mapSize; i++)
	{
		delete[] movementMap[i];
	}
	delete[] movementMap;
}

template <class T>
struct Node
{
	T* t;
	Node* left;
	Node* right;
	Node(T* tmp) : t(tmp), left(nullptr), right(nullptr) {}
	~Node()
	{
		if (left != nullptr)
		{
			left->right = nullptr;
			delete left;
		}
		if (right != nullptr)
		{
			right->left = nullptr;
			delete right;
		}
	}
	void putRight(T* t)
	{
		if (this->right != nullptr)
			this->right->putRight(t);
		else
		{
			this->right = new Node<T>(t);
			this->right->left = this;
		}
	}
	Node<T>* goLeft() { if (left != nullptr) return left->goLeft(); else return this; }
	void clearCoord()
	{
		Node<T>* cleaner = goLeft();
		while (cleaner != nullptr) {
			delete cleaner->t;
			cleaner = cleaner->right;
		}
	}
};

void generateValues(int& x, int& y, const int& mapSize, Node<Coord>*& curNode, char**& movementMap, int**& mapLen)
{
	int value = mapLen[x][y];
	if (x - 1 >= 0)
	{
		if (mapLen[x - 1][y] == INT_MAX && movementMap[x - 1][y] != '#' && movementMap[x - 1][y] != 'c' && movementMap[x - 1][y] != '!')
		{
			mapLen[x - 1][y] = value + 1;
			Coord* c = new Coord(x - 1, y);
			curNode->putRight(c);
		}
	}
	if (x + 1 < mapSize )
	{
		if (mapLen[x + 1][y] == INT_MAX && movementMap[x + 1][y] != '#' && movementMap[x + 1][y] != 'c' && movementMap[x + 1][y] != '!')
		{
			mapLen[x + 1][y] = value + 1;
			Coord* c = new Coord(x + 1, y);
			curNode->putRight(c);
		}
	}
	if (y - 1 >= 0)
	{
		if (mapLen[x][y - 1] == INT_MAX && movementMap[x][y - 1] != '#' && movementMap[x][y - 1] != 'c' && movementMap[x][y - 1] != '!')
		{
			mapLen[x][y - 1] = value + 1;
			Coord* c = new Coord(x, y - 1);
			curNode->putRight(c);
		}
	}
	if (y + 1 < mapSize)
	{
		if (mapLen[x][y + 1] == INT_MAX && movementMap[x][y + 1] != '#' && movementMap[x][y + 1] != 'c' && movementMap[x][y + 1] != '!')
		{
			mapLen[x][y + 1] = value + 1;
			Coord* c = new Coord(x, y + 1);
			curNode->putRight(c);
		}
	}
}

Direction* getPath(int& fromX, int& fromY, int& toX, int& toY, const int& movepoints, char**& movementMap, const int& mapSize, int& pathSize)
{
	Direction* path = new Direction[movepoints];
	int** mapLen = new int* [mapSize];
	for (int i = 0; i < mapSize; i++)
	{
		mapLen[i] = new int[mapSize];
		for (int j = 0; j < mapSize; j++)
		{
			mapLen[i][j] = INT_MAX;
		}
	}

	mapLen[fromX][fromY] = 0;
	Coord* coordHelper = new Coord(fromX, fromY);
	Node<Coord>* curNode = new Node<Coord>(coordHelper);
	Node<Coord>* startNode = curNode;
	int x, y;
	while (curNode != nullptr)
	{
		x = curNode->t->x;  y = curNode->t->y;
		if (abs(x - toX) == 1 && (y - toY) == 0 || abs(y - toY) == 1 && (x - toX) == 0)
			break;
		generateValues(x, y, mapSize, curNode, movementMap, mapLen);
		curNode = curNode->right;
		if (curNode == nullptr)
		{
			x = toX, y = toY;
		}
	}
	Direction dir = left;
	Node<Direction>* nullDirNode = new Node<Direction>(&dir);
	Node<Direction>* curDirNode = nullDirNode;
	int i = 0;
	int size;
	if (x != toX || y != toY)
	{
		int value = mapLen[x][y];
		size = value < movepoints ? value : movepoints;
		while (true)
		{
			if (value == 0)
				break;
			if (x - 1 >= 0) 
			{
				if (value - 1 == mapLen[x - 1][y])
				{
					Direction d = right;
					Node<Direction>* newDirNode = new Node<Direction>(&d);
					newDirNode->left = curDirNode->left;
					newDirNode->right = curDirNode;
					curDirNode->left = newDirNode;
					curDirNode = newDirNode;
					x = x - 1;
					value--;
					continue;
				}
			}

			if (x + 1 < mapSize) 
			{
				if (value - 1 == mapLen[x + 1][y])
				{
					Direction d = left;
					Node<Direction>* newDirNode = new Node<Direction>(&d);
					newDirNode->left = curDirNode->left;
					newDirNode->right = curDirNode;
					curDirNode->left = newDirNode;
					curDirNode = newDirNode;
					value--;
					x = x + 1;
					continue;
				}
			}

			if (y - 1 >= 0) 
			{
				if (value - 1 == mapLen[x][y - 1])
				{
					Direction d = top;
					Node<Direction>* newDirNode = new Node<Direction>(&d);
					newDirNode->left = curDirNode->left;
					newDirNode->right = curDirNode;
					curDirNode->left = newDirNode;
					curDirNode = newDirNode;
					value--;
					y -= 1;
					continue;
				}
			}

			if (y + 1 < mapSize)
			{
				if (value - 1 == mapLen[x][y + 1])
				{
					Direction d = bottom;
					Node<Direction>* newDirNode = new Node<Direction>(&d);
					newDirNode->left = curDirNode->left;
					newDirNode->right = curDirNode;
					curDirNode->left = newDirNode;
					curDirNode = newDirNode;
					value--;
					y += 1;
					continue;
				}
			}
		}
		while (curDirNode != nullDirNode)
		{
			if (i == size)
				break;
			path[i] = *curDirNode->t;
			i++;
			curDirNode = curDirNode->right;
			pathSize = i;
		}
	}

	if (x == toX && y == toY)
	{
		delete[] path;
		pathSize = 0;
		path = nullptr;
	}

	delete nullDirNode;
	startNode->clearCoord();
	delete startNode;
	for (int i = 0; i < mapSize; i++)
	{
		delete[] mapLen[i];
	}
	delete[] mapLen;
	return path;
}

Direction* Enemy::goToPlayer(Player& player, const int& mapSize, char(*map)[Map::MAP_SIZE], char(*enemyMap)[Map::MAP_SIZE], int& pathSize)
{
	int pX = getCoordMap(player.getCoord().x), pY = getCoordMap(player.getCoord().y);
	if (abs(pX - getCoord().x) <= 1 && abs(pY - getCoord().y) <= 1 && getMovepoints() > 0)
	{
		hit(player);
		return nullptr;
	}

	if (abs(pX - getCoord().x) >= Map::VISION / 2 || abs(pY - getCoord().y) >= Map::VISION / 2)
	{
		setMovepoints(0);
		return nullptr;
	}

	if (getMovepoints() <= 0)
	{
		setMovepoints();
		return nullptr;
	}
	
	char** movementMap = createMovementMap(mapSize, map, enemyMap);

	Coord enemyCoord = this->getCoord();
	Direction* path = getPath(enemyCoord.x, enemyCoord.y, pX, pY, getMovepoints(), movementMap, mapSize, pathSize);
	if (path == nullptr)
		setMovepoints(0);
	deleteMovementMap(movementMap, mapSize);
	return path;
	/*if (pX - getCoord().x > 0) 
	{ 
		move(right, mapSize); 
		return; 
	}
	if (pX - getCoord().x < 0) 
	{
		move(left, mapSize); return;
	}
	if (pY - getCoord().y > 0) 
	{ 
		move(top, mapSize);
		return; 
	}
	if (pY - getCoord().y < 0) 
	{
		move(bottom, mapSize);
		return; 
	}*/
}
//--------------------------------------------------BOT LOGIC

Enemy* Enemy::getEnemy(int x, int y)
{
	for (Enemy* e : enemys)
	{
		Coord c = e->getCoord();
		if (c.x == x && c.y == y)
		{
			if (e->getHp() > 0)
				return e;
			else
				return nullptr;
		}
	}
	return nullptr;
}
