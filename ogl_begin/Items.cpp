#include "Items.h"
#include "Creatures.h"
#include <string>

std::string EnchantedWeapon::getString()
{
	std::string string = "Enchanted Weapon: ";
	string.append(std::to_string(getDmg()));
	string.append("; effective vs ");
	switch (getContra())
	{
	case CreatureType::demonic:
		string.append("demons.");
		break;
	case CreatureType::humanic:
		string.append("humans.");
		break;
	case CreatureType::icy:
		string.append("ices.");
		break;
	case CreatureType::zombie:
		string.append("zombies.");
		break;
	case CreatureType::zoo:
		string.append("zoo.");
		break;
	}
	return string;
}

int EnchantedWeapon::getDmgTo(CreatureType creature)
{
	return creature == getContra() ? getDmg() * getKoeff() : getDmg();
}

int EnchantedArtifactWeapon::getDmgTo(CreatureType creature)
{
	return creature == getContra() ? getDmg() * getKoeff() : getDmg();
}

std::string EnchantedArtifactWeapon::getString()
{
	std::string string = "Enchanted ";
	string.append(ArtifactWeapon::getString());
	string.append(" Effective vs ");
	switch (getContra())
	{
	case CreatureType::demonic:
		string.append("demons.");
		break;
	case CreatureType::humanic:
		string.append("humans.");
		break;
	case CreatureType::icy:
		string.append("ices.");
		break;
	case CreatureType::zombie:
		string.append("zombies.");
		break;
	case CreatureType::zoo:
		string.append("zoo.");
		break;
	}
	return string;
}

std::string Item::getString()
{
	std::string string = "Masterkey.";
	return string;
}

void Item::saveToFile(std::ofstream& fout)
{
	LoadingItemType lit = mstrk;
	fout.write((char*)&lit, sizeof(int));
	fout.write((char*)this + sizeof(void*), sizeof(Item) - sizeof(void*));
}

void Item::loadFromFile(std::ifstream& fin)
{
	fin.read((char*)this + sizeof(void*), sizeof(Item) - sizeof(void*));
}

std::string ArtifactWeapon::getString()
{
	std::string string = "Artifact Weapon: ";
	string.append(std::to_string(getDmg()));
	string.append(". Agility: ");
	string.append(std::to_string(getAgil()));
	string.append(". Power: ");
	string.append(std::to_string(getPow()));
	string.append(". Int: ");
	string.append(std::to_string(getInt()));
	string.append(". Prot: ");
	string.append(std::to_string(getProtection()));
	string.append(".");
	return string;
}

std::string Protection::getString()
{
	std::string string = "";
	switch (getEquipmentType())
	{
	case EquipmentType::bib:
		string.append("Bib: ");
		break;
	case EquipmentType::helmet:
		string.append("Helmet: ");
		break;
	case EquipmentType::boots:
		string.append("Boots: ");
		break;
	case EquipmentType::pants:
		string.append("Pants: ");
		break;
	}
	string.append(std::to_string(getProtection()));
	string.append(".");
	return string;
}

void Protection::saveToFile(std::ofstream& fout)
{
	LoadingItemType lit = LoadingItemType::prt;
	fout.write((char*)&lit, sizeof(int));
	fout.write((char*)this + sizeof(void*), sizeof(Item) - sizeof(void*));
	fout.write((char*)this + sizeof(Item) + sizeof(void*), sizeof(Protection) - sizeof(void*) - sizeof(Item));
}

void Protection::loadFromFile(std::ifstream& fin)
{
	fin.read((char*)this + sizeof(void*), sizeof(Item) - sizeof(void*));
	fin.read((char*)this + sizeof(Item) + sizeof(void*), sizeof(Protection) - sizeof(void*) - sizeof(Item));
}

void ArtifactProtection::loadFromFile(std::ifstream& fin)
{
	fin.read((char*)this + sizeof(void*), sizeof(ArtifactProtection) - sizeof(void*));
}

std::string Weapon::getString()
{
	std::string string = "Weapon: ";
	string.append(std::to_string(getDmg()));
	string.append(".");
	return string;
}

void Weapon::saveToFile(std::ofstream& fout)
{
	LoadingItemType lit = wpn;
	fout.write((char*)&lit, sizeof(int));
	fout.write((char*)this + sizeof(void*), sizeof(Item) - sizeof(void*));
	fout.write((char*)this + sizeof(Item) + sizeof(void*), sizeof(Weapon) - sizeof(void*) - sizeof(Item));
}

void Potion::saveToFile(std::ofstream& fout)
{
	LoadingItemType lit = ptn;
	fout.write((char*)&lit, sizeof(int));
	fout.write((char*)this + sizeof(void*),sizeof(Potion) - sizeof(void*));
}

void Potion::loadFromFile(std::ifstream& fin)
{
	fin.read((char*)this + sizeof(void*),sizeof(Potion) - sizeof(void*));
}

void ArtifactProtection::saveToFile(std::ofstream& fout)
{
	LoadingItemType lit = aprt;
	fout.write((char*)&lit, sizeof(int));
	fout.write((char*)this + sizeof(void*), sizeof(ArtifactProtection) - sizeof(void*));
}

void ArtifactWeapon::saveToFile(std::ofstream& fout)
{
	LoadingItemType lit = awpn;
	fout.write((char*)&lit, sizeof(int));
	fout.write((char*)this + sizeof(void*),sizeof(ArtifactWeapon) - sizeof(void*));
}

void ArtifactWeapon::loadFromFile(std::ifstream& fin)
{
	fin.read((char*)this + sizeof(void*), sizeof(ArtifactWeapon) - sizeof(void*));
}

void EnchantedWeapon::saveToFile(std::ofstream& fout)
{
	LoadingItemType lit = ewpn;
	fout.write((char*)&lit, sizeof(int));
	fout.write((char*)this + sizeof(void*),sizeof(EnchantedWeapon) - sizeof(void*));
}

void EnchantedWeapon::loadFromFile(std::ifstream& fin)
{
	fin.read((char*)this + sizeof(void*), sizeof(EnchantedWeapon) - sizeof(void*));
}

void EnchantedArtifactWeapon::saveToFile(std::ofstream& fout)
{
	LoadingItemType lit = aewpn;
	fout.write((char*)&lit, sizeof(int));
	fout.write((char*)this + sizeof(void*),sizeof(EnchantedArtifactWeapon) - sizeof(void*));
}

void EnchantedArtifactWeapon::loadFromFile(std::ifstream& fin)
{
	fin.read((char*)this + sizeof(void*), sizeof(EnchantedArtifactWeapon) - sizeof(void*));
}

std::string Potion::getString()
{
	std::string string = "Potion: ";
	switch (getEffectType())
	{
	case EffectType::hp:
		string.append("HP: ");
		break;
	case EffectType::dmg:
		string.append("DMG: ");
		break;
	case EffectType::mana:
		string.append("MANA: ");
		break;
	}
	string.append(std::to_string(getEffect()));
	string.append(".");
	return string;
}

std::string ArtifactProtection::getString()
{
	std::string string = "";
	switch (getEquipmentType())
	{
	case EquipmentType::bib:
		string.append("Bib: ");
		break;
	case EquipmentType::helmet:
		string.append("Helmet: ");
		break;
	case EquipmentType::boots:
		string.append("Boots: ");
		break;
	case EquipmentType::pants:
		string.append("Pants: ");
		break;
	}
	string.append(std::to_string(getProtection()));
	string.append(". Dmg: ");
	string.append(std::to_string(getDmg()));
	string.append(". Agility: ");
	string.append(std::to_string(getAgil()));
	string.append(". Power: ");
	string.append(std::to_string(getPow()));
	string.append(". Int: ");
	string.append(std::to_string(getInt()));
	string.append(".");
	return string;
}

Item* loadItem(std::ifstream& fin)
{
	LoadingItemType lit = mstrk;
	fin.read((char*)&lit, sizeof(int));
	Item* item = nullptr;
	switch (lit)
	{
	case mstrk:
		item = new Item(masterkey);
		break;
	case LoadingItemType::ptn:
		item = new Potion(0, hp);
		break;
	case LoadingItemType::wpn:
		item = new Weapon();
		break;
	case LoadingItemType::prt:
		item = new Protection(helmet, 0);
		break;
	case LoadingItemType::aprt:
		item = new ArtifactProtection(helmet, 0, 0, 0, 0, 0, 0);
		break;
	case LoadingItemType::awpn:
		item = new ArtifactWeapon(0, 0, 0, 0, 0, 0);
		break;
	case LoadingItemType::ewpn:
		item = new EnchantedWeapon(0, 0, zombie);
		break;
	case LoadingItemType::aewpn:
		item = new EnchantedArtifactWeapon(0, 0, 0, 0, 0, 0, 0, zombie);
		break;
	}
	item->loadFromFile(fin);
	return item;
}

Equipment* loadEquipment(std::ifstream& fin)
{
	LoadingItemType lit = mstrk;
	fin.read((char*)&lit, sizeof(int));
	Equipment* equipment = nullptr;
	switch (lit)
	{
	case LoadingItemType::wpn:
		equipment = new Weapon();
		break;
	case LoadingItemType::prt:
		equipment = new Protection(helmet, 0);
		break;
	case LoadingItemType::aprt:
		equipment = new ArtifactProtection(helmet, 0, 0, 0, 0, 0, 0);
		break;
	case LoadingItemType::awpn:
		equipment = new ArtifactWeapon(0, 0, 0, 0, 0, 0);
		break;
	case LoadingItemType::ewpn:
		equipment = new EnchantedWeapon(0, 0, zombie);
		break;
	case LoadingItemType::aewpn:
		equipment = new EnchantedArtifactWeapon(0, 0, 0, 0, 0, 0, 0, zombie);
		break;
	}
	equipment->loadFromFile(fin);
	return equipment;
}

void Weapon::loadFromFile(std::ifstream& fin)
{
	fin.read((char*)this + sizeof(void*), sizeof(Item) - sizeof(void*));
	fin.read((char*)this + sizeof(Item) + sizeof(void*), sizeof(Weapon) - sizeof(void*) - sizeof(Item));
}
