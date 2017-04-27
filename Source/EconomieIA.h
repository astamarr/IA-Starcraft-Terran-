#pragma once

#include "IAManager.h"
#include "RecolteIA.h"


using namespace BWAPI;
class EconomieIA : public IAManager
{
	struct BuildOrder{
		BuildOrder::BuildOrder(UnitType Build, Unit BuildLoc, bool priority = false){
			Building = Build;
			BuildLocation = BuildLoc;
			HighPriority = priority;
			HasReserveRessources = false;
			IsCurrentlyBuilding = false;
			HasFinishedBuilding = false;
			HasReserveRessources = false;
		}
		UnitType Building;
		Unit BuildLocation;
		Unit AssignedBuilder;
		unsigned int LastTimeChecked = 0;
		bool HighPriority;
		bool IsCurrentlyBuilding ;
		bool HasFinishedBuilding ;
		bool HasReserveRessources;
	};


public:

	EconomieIA();
	~EconomieIA();
	void update();

	Unitset Caserne;





	UnitType supplyProviderType;
	std::list<BuildOrder*>  BuildingQueue;
	unsigned int CurrentFrame;
	Unit MainCommandCenter;
	int lastChecked = 0;
	bool SupplyBlocked = false;
	unsigned int ReservedMinerals = 0;
	unsigned int ReservedGas = 0;
	void onUnitCreate(BWAPI::Unit unit) override;
	void onUnitComplete(BWAPI::Unit unit) override;
	


private:
	void AddToBuildQueue(UnitType type, bool HighPriority = false);
	void CheckSupply();
	void TriggerBuildQueue();
	void Build( BuildOrder*Order);
	Fiche createFiche();
};

