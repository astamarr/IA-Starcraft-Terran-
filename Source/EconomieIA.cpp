#include "EconomieIA.h"

using namespace BWAPI;
using namespace Filter;

EconomieIA::EconomieIA()
{
	
	Broodwar << "c'est moddddddi" << std::endl;
	defaultBehaviour = new Fiche(Fiche::Type::BUILDING, BWAPI::UnitTypes::Terran_Refinery, BWAPI::UnitCommandTypes::Build);
	supplyProviderType = Broodwar->self()->getRace().getSupplyProvider();

	for (auto &u : Broodwar->self()->getUnits())
	{
		if (u->getType() == Broodwar->self()->getRace().getCenter())
		{
			MainCommandCenter = u;
		}
	}
	
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
	AddToBuildQueue(BWAPI::UnitTypes::Terran_Refinery, false);
	CurrentFrame = Broodwar->getFrameCount();


	if (MainCommandCenter->isIdle() && !MainCommandCenter->isTraining() && ReservedMinerals == 0 && !SupplyBlocked)
	{
		MainCommandCenter->train(supplyProviderType.whatBuilds().first);
	}


	if (Broodwar->self()->minerals() > 200 /*&& Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Barracks) == 0 */)
	{
		AddToBuildQueue(BWAPI::UnitTypes::Terran_Barracks,  false);
	}



	//On teste si on est en dèche de supply
	CheckSupply();

	//On essaye de construire les batiments de la liste d'attente
	TriggerBuildQueue();
	

	for (Unit n : Caserne)
	{
		if (n->isIdle() && !n->isTraining() && ReservedMinerals == 0 && !SupplyBlocked)
		{
			n->train(BWAPI::UnitTypes::Terran_Marine);
		}
	}
}



void EconomieIA::CheckSupply()
{

	if (SupplyBlocked == true)
	{
		if (BWAPI::Broodwar->self()->supplyTotal() > (BWAPI::Broodwar->self()->supplyUsed() + 4))
			{
				SupplyBlocked = false;
			}
	}
	
	else if (((BWAPI::Broodwar->self()->supplyTotal() - BWAPI::Broodwar->self()->supplyUsed()) <= 2) && Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
	{
		
		AddToBuildQueue(supplyProviderType, true);
		SupplyBlocked = true;

	}
}




void EconomieIA::AddToBuildQueue(UnitType type, bool HighPriority)
{
	if (SupplyBlocked && type != supplyProviderType)
	{
		return;
	}
	
	for (BuildOrder* &n : BuildingQueue)
	{
		
		if (n->Building == type)
		{
				return;
		}
	}
	BuildingQueue.push_back(new BuildOrder(type, MainCommandCenter, HighPriority));

}


// ça essaye de lancer toutes les constructions 
void  EconomieIA::TriggerBuildQueue()
{

	for (BuildOrder* n : BuildingQueue)
	{
		if (SupplyBlocked && n->Building != supplyProviderType)
		{
			continue;
		}

		if (n->LastTimeChecked == 0 || CurrentFrame > n->LastTimeChecked + 100)
		{
			
			 Build(n);
			if (n->HasFinishedBuilding)
			{
				BuildingQueue.remove(n);
				delete(n);

			}

		}
	}
}

void EconomieIA::Build( BuildOrder* Order)
{
	Order->LastTimeChecked = Broodwar->getFrameCount();


	if (Order->IsCurrentlyBuilding == true)
	{
		if (!Order->AssignedBuilder->isConstructing() )
		{
			
			Order->IsCurrentlyBuilding = false;
			Order->HasFinishedBuilding = true;
			if (Order->HasReserveRessources)
			{
				ReservedMinerals -= Order->Building.mineralPrice();
				ReservedGas -= Order->Building.gasPrice();
			}	
		}
		return;
	}

	//Check financier , si batiment prioritaire et pas assez de thunes ==> on lock les fonds. En fait on peut refactorer tout le code de reservedmineral par un simple bool...
	if (((Broodwar->self()->minerals() < Order->Building.mineralPrice()) || (Broodwar->self()->gas() < Order->Building.gasPrice())))
	{
		if (Order->HighPriority &&  !Order->HasReserveRessources)
		{
			ReservedMinerals += Order->Building.mineralPrice();
			ReservedGas += Order->Building.gasPrice();
			Order->HasReserveRessources = true;
		}
	
		return;
	}

	Unit Builder = Order->BuildLocation->getClosestUnit(GetType == Order->Building.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);
	if (Builder)
	{
		if (Order->Building.isBuilding())
		{
			TilePosition targetBuildLocation = Broodwar->getBuildLocation(Order->Building, Builder->getTilePosition());
			if (targetBuildLocation)
			{
				// Register an event that draws the target build location

				Broodwar->drawBoxMap(Position(targetBuildLocation),
					Position(targetBuildLocation + supplyProviderType.tileSize()),
					Colors::Blue);

			
				//Broodwar << Builder << std::endl;
				// Order the builder to construct the supply structure
			
				bool Success = Builder->build(Order->Building, targetBuildLocation);
				if (Success){
					Order->IsCurrentlyBuilding = true;
					Order->AssignedBuilder = Builder;

				}

			
			}
		}
	}
}

void EconomieIA::onUnitCreate(BWAPI::Unit unit)
{
	
}

 void EconomieIA::onUnitComplete(BWAPI::Unit unit)
 {
	 UnitType NewUnitType = unit->getType();
	
	 if (NewUnitType.isBuilding())
	 {
		
	

		 if (NewUnitType == BWAPI::UnitTypes::Terran_Barracks)
		 {
			 Caserne.emplace(unit);
		 }

	 }
 };

