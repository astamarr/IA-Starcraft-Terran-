#include "RecolteIA.h"


RecolteIA::RecolteIA()
{
	createFiche();
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

}
