#pragma once
#include "Map.h"
#include <fstream>

#define AGIL 0
#define POW 1
#define INT 2
#define DMG 3
#define PROT 4

	
	class Item
	{
	private:
		ItemType itemType;
	public:
		inline Item() : itemType(masterkey) {};
		inline Item(const ItemType type) : itemType(type) {}
		inline Item(Item& item) : itemType(item.itemType) {}
		virtual inline ~Item() {}
		inline ItemType getType() { return itemType; }
		virtual inline void operator = (const Item& item) { this->itemType = item.itemType; }

		virtual std::string getString();
		virtual void saveToFile(std::ofstream& fout);
		virtual void loadFromFile(std::ifstream& fin);
		//virtual void iterate(Player& player);
	};

	class Potion : public Item
	{
	private:
		int effect;
		EffectType type;
	public:
		inline Potion(int effect, EffectType type) : Item(potion), type(type), effect(effect) {}

		void saveToFile(std::ofstream& fout) override;
		void loadFromFile(std::ifstream& fin) override;
		inline EffectType getEffectType() { return type; }
		inline int getEffect() const { return effect; }
		std::string getString() override;
	};

	class Equipment
	{
	private:
		EquipmentType equipmentType;
	public:
		virtual void saveToFile(std::ofstream& fout) = 0;
		virtual void loadFromFile(std::ifstream& fin) = 0;
		inline Equipment(EquipmentType type) : equipmentType(type) {}
		inline Equipment() : equipmentType(weapon) {}
		virtual inline ~Equipment() {}
		inline EquipmentType getEquipmentType() { return equipmentType; }
		inline virtual int getDmg() const = 0;
		virtual int getProtection() const = 0;
		virtual int getInt() const = 0;
		virtual int getPow() const = 0;
		virtual int getAgil() const = 0;
	};

	class Artifact
	{
	private:
		int dCharacteristics[5];
	public:
		inline Artifact()
		{
			dCharacteristics[DMG] = 0;
			dCharacteristics[AGIL] = 0;
			dCharacteristics[INT] = 0;
			dCharacteristics[POW] = 0;
			dCharacteristics[PROT] = 0;
		}
		inline Artifact(int da, int dp, int di, int dd, int dprot)
		{
			dCharacteristics[AGIL] = da;
			dCharacteristics[POW] = dp;
			dCharacteristics[INT] = di;
			dCharacteristics[DMG] = dd;
			dCharacteristics[PROT] = dprot;
		}
		virtual inline ~Artifact() {}
		inline const int* getDCharacteristics() const { return dCharacteristics; }
	};

	class Weapon : public Item, public Equipment
	{
	private:
		int damage;
		bool isEnchanted;
	public:
		inline Weapon() : Item(equipment), Equipment(), damage(10), isEnchanted(false) {}
		inline Weapon(int dmg) : Item(equipment), damage(dmg), Equipment(), isEnchanted(false) {}
		virtual inline ~Weapon() {}

		void loadFromFile(std::ifstream& fin) override;
		void saveToFile(std::ofstream& fout) override;
		inline bool getIsEnchanted() const { return isEnchanted; }
		inline int getProtection() const override { return 0; }
		inline int getDmg() const override { return damage; }
		inline int getAgil() const override { return 0; }
		inline int getInt() const override { return 0; }
		inline int getPow() const override { return 0; }
		std::string getString() override;
	protected:
		inline Weapon(int dmg, bool ench) : Item(equipment), damage(dmg), Equipment(), isEnchanted(ench) {}
		inline Weapon(bool ench) : Item(equipment), Equipment(), isEnchanted(ench) {}
	};

	class Protection : public Item, public Equipment
	{
	private:
		int protection;
	public:
		void saveToFile(std::ofstream& fout) override;
		void loadFromFile(std::ifstream& fin) override;
		inline Protection(EquipmentType type, int prot) : Item(equipment), Equipment(type), protection(prot) {}
		virtual inline ~Protection() {}
 		inline int getProtection() const override { return protection; }
		inline int getDmg() const override { return 0; }
		inline int getAgil() const override { return 0; }
		inline int getInt() const override { return 0; }
		inline int getPow() const override { return 0; }
		std::string getString() override;
	};

	class ArtifactProtection : public Protection, public Artifact
	{
	public:
		void saveToFile(std::ofstream& fout) override;
		void loadFromFile(std::ifstream& fin) override;
		inline ArtifactProtection(EquipmentType type, int prot, int da, int dp, int di, int dd, int dprot) : Protection(type, prot), Artifact(da, dp, di, dd, dprot) {}
		inline int getProtection() const override { return Protection::getProtection() + getDCharacteristics()[PROT]; }
		inline int getDmg() const override { return getDCharacteristics()[DMG]; }
		inline int getAgil() const override { return getDCharacteristics()[AGIL]; }
		inline int getInt() const override { return getDCharacteristics()[INT]; }
		inline int getPow() const override { return getDCharacteristics()[POW]; }
		std::string getString() override;
	};

	class ArtifactWeapon : public virtual Weapon, public Artifact
	{
	private:
		
	public:
		void saveToFile(std::ofstream& fout) override;
		void loadFromFile(std::ifstream& fin) override;
		inline ArtifactWeapon() : Weapon(), Artifact() {}
		inline ArtifactWeapon(int dmg, int da, int dp, int di, int dd, int dprot) : Weapon(dmg), Artifact(da, dp, di, dd, dprot) {}
		virtual inline ~ArtifactWeapon() {}
		inline int getProtection() const override { return getDCharacteristics()[PROT]; }
		inline int getDmg() const override { return Weapon::getDmg() + getDCharacteristics()[DMG]; }
		inline int getAgil() const override { return getDCharacteristics()[AGIL]; }
		inline int getInt() const override { return getDCharacteristics()[INT]; }
		inline int getPow() const override { return getDCharacteristics()[POW]; }
		std::string getString() override;
	};

	class Enchantment
	{
	private:
		CreatureType contra;
		float koeff;
	protected:
		inline Enchantment(float k, CreatureType type) : koeff(k), contra(type) {}
		virtual inline ~Enchantment() {}
		inline CreatureType getContra() const { return contra; }
		inline float getKoeff() const { return koeff; }
	};

	class EnchantedWeapon : public virtual Weapon, public Enchantment
	{
	public:
		void saveToFile(std::ofstream& fout) override;
		void loadFromFile(std::ifstream& fin) override;
		inline EnchantedWeapon(int dmg, float k, CreatureType gainAgainst) : Weapon(dmg, true), Enchantment(abs(k), gainAgainst) {}
		virtual inline ~EnchantedWeapon() {}
		virtual int getDmgTo(CreatureType creature);
		std::string getString() override;
	protected:
		inline EnchantedWeapon(float k, CreatureType gainAgainst) : Weapon(true), Enchantment(abs(k), gainAgainst) {}
	};

	class EnchantedArtifactWeapon : public EnchantedWeapon, public ArtifactWeapon
	{
	private:

	public:
		void saveToFile(std::ofstream& fout) override;
		void loadFromFile(std::ifstream& fin) override;
		inline EnchantedArtifactWeapon(int dmg, int da, int dp, int di, int dd, int dprot, float k, CreatureType gainAgainst) : Weapon(dmg, true), ArtifactWeapon(dmg, da, dp, di, dd, dprot), EnchantedWeapon(dmg, k, gainAgainst) {}
		inline int getProtection() const override { return getDCharacteristics()[PROT]; }
		inline int getDmg() const override { return Weapon::getDmg() + getDCharacteristics()[DMG]; }
		inline int getAgil() const override { return getDCharacteristics()[AGIL]; }
		inline int getInt() const override { return getDCharacteristics()[INT]; }
		inline int getPow() const override { return getDCharacteristics()[POW]; }
		int getDmgTo(CreatureType creature) override;
		std::string getString() override;
	};

	Item* loadItem(std::ifstream& fin);
	Equipment* loadEquipment(std::ifstream& fin);