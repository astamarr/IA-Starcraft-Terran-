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

	CurrentFrame = Broodwar->getFrameCount();


	if (MainCommandCenter->isIdle() && !MainCommandCenter->isTraining() && ReservedMinerals == 0 && !SupplyBlocked)
	{
		MainCommandCenter->train(supplyProviderType.whatBuilds().first);
	}


	if (Broodwar->self()->minerals() > 500)
	{
		BuildingQueue.push_back(new BuildOrder(BWAPI::UnitTypes::Terran_Barracks, MainCommandCenter, false));
	}

	//On teste si on est en dèche de supply
	CheckSupply();

	//On essaye de construire les batiments de la liste d'attente
	TriggerBuildQueue();
}



void EconomieIA::CheckSupply()
{

	if (SupplyBlocked == true)
	{
		if (BWAPI::Broodwar->self()->supplyTotal() > (BWAPI::Broodwar->self()->supplyUsed() + 2))
			{
				SupplyBlocked = false;
			}
	}
	
	if (((BWAPI::Broodwar->self()->supplyTotal() - BWAPI::Broodwar->self()->supplyUsed()) <= 2) && Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
	{
		
		Broodwar << "OULALA C LA DECHE" << std::endl;
		Broodwar << BWAPI::Broodwar->self()->supplyTotal() << std::endl;
		Broodwar << "-" << std::endl;
		Broodwar << BWAPI::Broodwar->self()->supplyUsed() << std::endl;
		// ça fait moins chier que de refaire un foutu compteur de frames pour éviter le flood...
		for (BuildOrder* n : BuildingQueue)
		{
			if (n->Building == supplyProviderType)
			{
				Broodwar << "ON ON VIENT DE SAUTER OUF " << std::endl;
				return;
			}
			
		}
		Broodwar << "ON AJOUTE" << std::endl;
		// on ajoute un ordre de construction prioritaire de supply a la queue
		SupplyBlocked = true;
		BuildingQueue.push_back(new BuildOrder(supplyProviderType, MainCommandCenter, true));
	}
}

// ça essaye de lancer toutes les constructions 
void  EconomieIA::TriggerBuildQueue()
{

	for (BuildOrder* n : BuildingQueue)
	{
		if (n->LastTimeChecked == 0 || CurrentFrame > n->LastTimeChecked + 100)
		{
			bool success = Build(n);
			if (success)
			{
				BuildingQueue.remove(n);
				delete(n);

			}

		}
	}
}

bool EconomieIA::Build( BuildOrder* Order)
{
	Order->LastTimeChecked = Broodwar->getFrameCount();

	//Check financier , si batiment prioritaire et pas assez de thunes ==> on lock les fonds. En fait on peut refactorer tout le code de reservedmineral par un simple bool...
	if ((Broodwar->self()->minerals() < Order->Building.mineralPrice()) || (Broodwar->self()->gas() < Order->Building.gasPrice()))
	{
		if (Order->HighPriority &&  !Order->HasReserveRessources)
		{
			ReservedMinerals += Order->Building.mineralPrice();
			ReservedGas += Order->Building.gasPrice();
			Broodwar << " ON RESERVE " << ReservedMinerals << std::endl;
			Order->HasReserveRessources = true;
		}
		Broodwar << " j'ai pas de sous " << std::endl;
		return false;
	}

	Unit supplyBuilder = Order->BuildLocation->getClosestUnit(GetType == Order->Building.whatBuilds().first && (IsIdle || IsGatheringMinerals) && IsOwned);
	if (supplyBuilder)
	{
		if (supplyProviderType.isBuilding())
		{
			TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
			if (targetBuildLocation)
			{
				// Register an event that draws the target build location

				Broodwar->drawBoxMap(Position(targetBuildLocation),
					Position(targetBuildLocation + supplyProviderType.tileSize()),
					Colors::Blue);

				Broodwar << "BUILD OMG " << std::endl;
				Broodwar << supplyBuilder << std::endl;
				// Order the builder to construct the supply structure
		
				supplyBuilder->build(Order->Building, targetBuildLocation);
					Order->IsCurrentlyBuilding = true;

					if (Order->HasReserveRessources)
					{
						ReservedMinerals -= Order->Building.mineralPrice();
						ReservedGas -= Order->Building.gasPrice();
					}
					return true;

				Broodwar << "WALA " << std::endl;
			}
		}
		return false;
	}
}

