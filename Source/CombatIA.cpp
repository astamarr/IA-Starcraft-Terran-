#include "CombatIA.h"


CombatIA::CombatIA()
{
}


CombatIA::~CombatIA()
{
}

Fiche CombatIA::createFiche()
{
	Fiche fiche(Fiche::Type::ATTACK, BWAPI::UnitTypes::Protoss_Dark_Templar, BWAPI::UnitCommandTypes::Attack_Unit);

	return fiche;
}

void CombatIA::update()
{

}