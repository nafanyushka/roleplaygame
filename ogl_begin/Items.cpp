#include "Items.h"
#include "Creatures.h"

int EnchantedWeapon::getDmgTo(CreatureType creature)
{
	return creature == getContra() ? getDmg() * getKoeff() : getDmg();
}

int EnchantedArtifactWeapon::getDmgTo(CreatureType creature)
{
	return creature == getContra() ? getDmg() * getKoeff() : getDmg();
}
