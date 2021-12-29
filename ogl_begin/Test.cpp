#include <gtest/gtest.h>
#include <iostream>
#include "Environment.h"
#include "Creatures.h"
#include "Items.h"
#include "Map.h"

TEST(PlayerTest, PlayerTest)
{
	Player player(100, 10, 0, 0, humanic, Coord(0, 0));
	player.move(Direction::top, 100);
	EXPECT_EQ(player.getCoord().x, 0);
	EXPECT_EQ(player.getCoord().y, 1);
	player.move(Direction::bottom, 100);
	EXPECT_EQ(player.getCoord().x, 0);
	EXPECT_EQ(player.getCoord().y, 0);
	player.move(Direction::right, 100);
	EXPECT_EQ(player.getCoord().x, 1);
	EXPECT_EQ(player.getCoord().y, 0);
	player.move(Direction::left, 100);
	EXPECT_EQ(player.getCoord().x, 0);
	EXPECT_EQ(player.getCoord().y, 0);

	Enemy enemy(10, 10, 5, 4, zombie, Coord(0, 1));
	player.hit(enemy);
	EXPECT_EQ(enemy.getHp(), 5);
	player.hit(enemy);
	EXPECT_EQ(enemy.getHp(), 0);
	EXPECT_EQ(player.getLvl(), 2);
	enemy.setHp(10);

	player.setDamage(0);

	Weapon* weapon = new EnchantedArtifactWeapon(5, 0, 0, 0, 0, 0, 2.0f, zombie);
	player.pickup(weapon);
	player.equip(0);
	player.hit(enemy);
	EXPECT_EQ(enemy.getHp(), 0);

	player.heal(10);
	EXPECT_EQ(player.getHp(), 100);

	enemy.hit(player);
	EXPECT_EQ(player.getHp(), 90);
	player.heal(10);
	EXPECT_EQ(player.getHp(), 100);

	player.setMovepoints();
	EXPECT_EQ(player.getMovepoints(), 2);
	enemy.setMovepoints();
	EXPECT_EQ(enemy.getMovepoints(), -1);
	enemy.setMovepoints();
	EXPECT_EQ(enemy.getMovepoints(), 2);
	enemy.move(right, 100);
	EXPECT_EQ(enemy.getCoord().x, 1);
	EXPECT_EQ(enemy.getCoord().y, 1);
	EXPECT_EQ(enemy.getMovepoints(), 1);

	Enemy demonEnemy(10, 5, 5, 4, demonic, Coord(1, 0));
	player.hit(demonEnemy);
	EXPECT_EQ(demonEnemy.getHp(), 5);

	player.setMovepoints(0);
	player.setMovepoints();
	EXPECT_EQ(player.getMovepoints(), 2);
	player.upStats(agil);
	player.setMovepoints(0);
	player.setMovepoints();
	EXPECT_EQ(player.getMovepoints(), 3);

	player.upLvl();
	EXPECT_EQ(player.getLvl(), 3);
	player.setExp(8);
	player.upLvl();
	EXPECT_EQ(player.getLvl(), 4);

	Item** potions = new Item*[8];
	for (int i = 0; i < 8; i++) 
	{
		potions[i] = new Potion(3, EffectType::hp);
		player.pickup(potions[i]);
		EXPECT_EQ(player.getItems(), i + 1);
	}
	Item* item = new Item();
	player.pickup(item);
	EXPECT_EQ(player.getItems(), 8);

	player.setHp(5);
	player.equip(0);
	EXPECT_EQ(player.getItems(), 7);
	EXPECT_EQ(player.getHp(), 8);
	player.pickup(item);
	EXPECT_EQ(player.getItems(), 8);
	
	Door* door = new Door(Coord(0, 1), true, 1);
	door->iterate(player);
	EXPECT_EQ(player.getItems(), 8);
	EXPECT_EQ(door->getStatus(), false);
	delete door;
	door = new Door(Coord(0, 1), true, 100);
	door->iterate(player);
	EXPECT_EQ(player.getItems(), 7);
	EXPECT_EQ(door->getStatus(), true);
	delete door;
	
	Protection* protection = new Protection(helmet, 10);
	player.pickup(protection);
	player.equip(7);
	EXPECT_EQ(player.getItems(), 7);
	EXPECT_FALSE(player.getEquipment()[HELMET] == nullptr);

	ArtifactProtection* ap = new ArtifactProtection(bib, 1, 1, 1, 1, 1, 0);
	player.pickup(ap);
	player.equip(7);
	EXPECT_EQ(player.getItems(), 7);
	EXPECT_FALSE(player.getEquipment()[BIB] == nullptr);
	player.setMovepoints(0);
	player.setMovepoints();
	EXPECT_EQ(player.getMovepoints(), 4);
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}