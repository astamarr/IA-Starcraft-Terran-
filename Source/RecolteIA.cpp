#include "RecolteIA.h"

using namespace BWAPI;
using namespace Filter;

RecolteIA::RecolteIA()
{
	ficheMinerals = new Fiche(Fiche::Type::GATHERING, BWAPI::UnitTypes::Resource_Mineral_Field, BWAPI::UnitCommandTypes::Gather);
	ficheGas = new Fiche(Fiche::Type::GATHERING, BWAPI::UnitTypes::Resource_Vespene_Geyser, BWAPI::UnitCommandTypes::Gather);
	defaultBehaviour = ficheMinerals;
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
	setRessourcesRequired(Broodwar->self()->minerals(), Broodwar->self()->gas());

	for (Unit u : assignedUnits)
	{
		if (u->isIdle())
		{
			if (u->isCarryingGas() || u->isCarryingMinerals())
			{
				u->returnCargo();
			}
			else if (!u->getPowerUp())  // The worker cannot harvest anything if it is carrying a powerup such as a flag
			{
				// Harvest from the nearest mineral patch or gas refinery
				BWAPI::PtrUnitFilter rsc = (minerals <= gas) ? BWAPI::Filter::IsMineralField : BWAPI::Filter::IsRefinery;
				if (!u->gather(u->getClosestUnit(rsc)))
				{
					//default
					if (!u->gather(u->getClosestUnit(IsMineralField || IsRefinery)))
						Broodwar << Broodwar->getLastError() << std::endl;
				}
			} // closure: has no powerup
		} // closure: if idle
	}
}

void RecolteIA::setRessourcesRequired(short minerals_t, short gas_t)
{
	minerals = minerals_t;
	gas = gas_t;
	defaultBehaviour = (minerals > gas) ? ficheMinerals : ficheGas;

}