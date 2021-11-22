#pragma once

#define AGIL 0
#define POW 1
#define INT 2
#define DMG 3

	enum ItemType
	{
		equipment,
		potion,
		masterkey
	};

	enum EquipmentType
	{
		weapon,
		helmet,
		bib,
		pants,
		boots
	};

	class Item
	{
	private:
		ItemType itemType;
	public:
		inline Item() : itemType(masterkey) {};
		inline Item(const ItemType type) : itemType(type) {}
		inline Item(Item& item) : itemType(item.itemType) {}
		inline ItemType getType() { return itemType; }
		virtual inline void operator = (const Item& item) { this->itemType = item.itemType; }

		//virtual void iterate(Player& player);
	};

	class Equipment
	{
	private:
		EquipmentType equipmentType;
	public:
		inline Equipment(EquipmentType type) : equipmentType(type) {}
		inline Equipment() : equipmentType(weapon) {}
		inline EquipmentType getEquipmentType() { return equipmentType; }
		inline virtual int getDmg() const = 0;
		virtual int getInt() const = 0;
		virtual int getPow() const = 0;
		virtual int getAgil() const = 0;
	};

	class Enchantment
	{
	private:
		int dCharacteristics[4];
	public:
		inline Enchantment()
		{
			dCharacteristics[DMG] = 0;
			dCharacteristics[AGIL] = 0;
			dCharacteristics[INT] = 0;
			dCharacteristics[POW] = 0;
		}
		inline Enchantment(int da, int dp, int di, int dd)
		{
			dCharacteristics[AGIL] = da;
			dCharacteristics[POW] = dp;
			dCharacteristics[INT] = di;
			dCharacteristics[DMG] = dd;
		}
		inline const int* getDCharacteristics() const { return dCharacteristics; }
	};

	class Weapon : public Item, public Equipment
	{
	private:
		int damage;
		Enchantment* enchantment;
	public:
		inline Weapon() : Item(equipment), Equipment(), enchantment(nullptr), damage(10) {}
		inline Weapon(int dmg) : Item(equipment), damage(dmg), Equipment(), enchantment(nullptr) {}
		inline Weapon(int dmg, Enchantment* ench) : Item(equipment), damage(dmg), Equipment(), enchantment(ench) {}
		inline int getDmg() const override { return damage + (enchantment == nullptr ? 0 : enchantment->getDCharacteristics()[DMG]); }
		inline int getAgil() const override { return enchantment == nullptr ? 0 : enchantment->getDCharacteristics()[AGIL]; }
		inline int getInt() const override { return enchantment == nullptr ? 0 : enchantment->getDCharacteristics()[INT]; }
		inline int getPow() const override { return enchantment == nullptr ? 0 : enchantment->getDCharacteristics()[POW]; }
	};
