#include "RecolteIA.h"


RecolteIA::RecolteIA()
{
	defaultBehaviour = new Fiche(Fiche::Type::GATHERING, BWAPI::UnitTypes::Resource_Mineral_Field, BWAPI::UnitCommandTypes::Gather);
}


RecolteIA::~RecolteIA()
{
}


Fiche RecolteIA::createFiche()
{
	Fiche fiche(Fiche::Type::GATHERING, BWAPI::UnitTypes::Resource_Mineral_Field, BWAPI::UnitCommandTypes::Gather);

	return fiche;
}

void RecolteIA::update()
{
	for (BWAPI::Unitset::iterator i = assignedUnits.begin(); i != assignedUnits.end(); ++i)
	{
		const BWAPI::Unit& unit = (*i);
		findBestPosition(unit);
	}		
}

void RecolteIA::setRessourcesRequired(short minerals_t, short gas_t)
{
	minerals = minerals_t;
	gas = gas_t;
}