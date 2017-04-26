#pragma once

#include "IAManager.h"


using namespace BWAPI;
class EconomieIA : public IAManager
{
	struct BuildOrder{
		BuildOrder::BuildOrder(UnitType Build, Unit BuildLoc, bool priority = false){
			Building = Build;
			BuildLocation = BuildLoc;
			HighPriority = priority;
			HasReserveRessources = false;
		}
		UnitType Building;
		Unit BuildLocation;
		unsigned int LastTimeChecked = 0;
		bool HighPriority;
		bool IsCurrentlyBuilding;
		bool HasReserveRessources;
	};


public:
	UnitType supplyProviderType;
	std::list<BuildOrder*>  BuildingQueue;
	unsigned int CurrentFrame;

	Unit MainCommandCenter;
	int lastChecked = 0;
	bool SupplyBlocked = false;
	unsigned int ReservedMinerals = 0;
	unsigned int ReservedGas = 0;
	
	EconomieIA();
	
	~EconomieIA();
	void update();

private:
	void EconomieIA::CheckSupply();
	void  EconomieIA::TriggerBuildQueue();
	bool EconomieIA::Build( BuildOrder* Order);
	Fiche createFiche();
};

