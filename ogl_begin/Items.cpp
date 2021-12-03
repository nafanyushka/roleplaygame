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

std::string Weapon::getString()
{
	std::string string = "Weapon: ";
	string.append(std::to_string(getDmg()));
	string.append(".");
	return string;
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