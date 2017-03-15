#include "EconomieIA.h"


EconomieIA::EconomieIA()
{
	
}


EconomieIA::~EconomieIA()
{
}

Fiche EconomieIA::createFiche()
{
	Fiche fiche(Fiche::Type::BUILDING, BWAPI::UnitTypes::Terran_Refinery, BWAPI::UnitCommandTypes::Build);
	return fiche;
}
