#pragma once

#include "IAManager.h"

class RecolteIA : public IAManager
{
public:
	RecolteIA();
	~RecolteIA();
	void update();

	void setRessourcesRequired(short, short);
private:
	Fiche createFiche();
	short minerals;
	short gas;
};

/*
BWAPI::Unitset myUnits = BWAPI::Broodwar->self()->getUnits();
for ( auto u : myUnits )
{
	if ( u->getType().isRefinery() )
	{
		int nWorkersAssigned = u->getClientInfo<int>('work');
		if ( nWorkersAssigned < 3 )
		{
			Unit pClosestIdleWorker = u->getClosestUnit(BWAPI::Filter::IsWorker && BWAPI::Filter::IsIdle);
			if ( pClosestIdleWorker )
			{
			// gather from the refinery (and check if successful)
			if ( pClosestIdleWorker->gather(u) )
			{
			// set a back reference for when the unit is killed or re-assigned (code not provided)
			pClosestIdleWorker->setClientInfo(u, 'ref');
			// Increment the number of workers assigned and associate it with the refinery
			++nWorkersAssigned;
			u->setClientInfo(nWorkersAssigned, 'work');
			}
			}
		} // workers < 3
	} // isRefinery
} // for
*/
