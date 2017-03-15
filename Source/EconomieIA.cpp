#include "EconomieIA.h"


EconomieIA::EconomieIA()
{
	defaultBehaviour = new Fiche(Fiche::Type::BUILDING, BWAPI::UnitTypes::Terran_Refinery, BWAPI::UnitCommandTypes::Build);
}

EconomieIA::~EconomieIA()
{
}

Fiche EconomieIA::createFiche()
{
	Fiche fiche(Fiche::Type::BUILDING, BWAPI::UnitTypes::Terran_Refinery, BWAPI::UnitCommandTypes::Build);
	return fiche;
}

void EconomieIA::update()
{
	for (BWAPI::Unitset::iterator i = assignedUnits.begin(); i != assignedUnits.end(); ++i)
	{
		const BWAPI::Unit& unit = (*i);
		findBestPosition(unit);
	}
}
