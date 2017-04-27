#include "CombatIA.h"


CombatIA::CombatIA()
{
	defaultBehaviour = new Fiche(Fiche::Type::ATTACK, BWAPI::UnitTypes::AllUnits, BWAPI::UnitCommandTypes::Attack_Unit);
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
	for (BWAPI::Unitset::iterator i = assignedUnits.begin(); i != assignedUnits.end(); ++i)
	{
		const BWAPI::Unit& unit = (*i);
		findBestPosition(unit);
	}
}



