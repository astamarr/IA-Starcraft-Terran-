#include "StrategyManager.h"

using namespace BWAPI;
using namespace Filter;

StrategyManager::StrategyManager()
{
	//décisions de construction, research, etc..
	EconomieManager = new EconomieIA();
	Managers.push_back(EconomieManager);
	//décision de combat, production d'unité, upgarde, ...
	fightsManager = new CombatIA();
	Managers.push_back(fightsManager);
	//décisions de planification de récoltes 
	gathererManager = new RecolteIA();
	Managers.push_back(gathererManager);

	for (TilePosition n : Broodwar->getStartLocations())
	{
		if (n != Broodwar->self()->getStartLocation())
		{
			PotentialEnnemyBases.push_back(n);
		}

	}

	//PotentialEnnemyBases = Broodwar->getStartLocations();
	//PotentialEnnemyBases.erase(Broodwar->self()->getStartLocation());

	for (TilePosition &n : PotentialEnnemyBases)
	{
		Broodwar << "Title :  " << n.x << "  / " << n.y  <<std::endl;


	}


	
}

StrategyManager::~StrategyManager()
{
	delete EconomieManager;
	delete fightsManager;
	delete gathererManager;
}

void StrategyManager::update(const int& framesCount)
{
	Unitset units = Broodwar->self()->getUnits();
	

	if (Broodwar->getFrameCount() % Broodwar->getLatencyFrames() != 0)
		return;

	for (auto &u : units)
	{
		if (!u->exists())
			continue;
		if (u->isLockedDown() || u->isMaelstrommed() || u->isStasised())
			continue;
		if (u->isLoaded() || !u->isPowered() || u->isStuck())
			continue;
		if (!u->isCompleted() || u->isConstructing())
			continue;
		
		if (u->getType().isWorker() && !u->isPatrolling())
		{
			workers.emplace(u);
		}
		else if (u->getType().isResourceDepot()) // A resource depot is a Command MainCommandCenter, Nexus, or Hatchery
		{
			buildings.emplace(u);
		}

	} // closure: unit iterator

	fightsManager->assignUnits(fighters);
	gathererManager->assignUnits(workers);
	EconomieManager->assignUnits(buildings);
	fightsManager->update();
	gathererManager->update();
	EconomieManager->update();







	Scouting();







}


void StrategyManager::Scouting(){
	
	// set the first scout
	if ((Scouts.size() == 0) && Broodwar->self()->supplyUsed() > 18){

		for (Unit n : workers)
		{
			if (n->isIdle() || n->isGatheringMinerals())
			{
				Broodwar << " AADDD SCOUT " << std::endl;
				Scouts.emplace(n);
				workers.erase(n);
				return;

			}

		}

	}

	for (Unit n : Scouts)
	{
		Unitset Eyes = n->getUnitsInRadius(500, BWAPI::Filter::IsBuilding &&BWAPI::Filter::IsEnemy);
		//Get data from scouts
		if (Eyes.size() > 0)
		{
			EnnemyBase = PotentialEnnemyBases.at(BaseChecked);
			Broodwar << "ENNEMY BASE FOUND AT  : " << EnnemyBase.x << EnnemyBase.y <<  std::endl;
			n->move(Position(Broodwar->self()->getStartLocation().x * 32, Broodwar->self()->getStartLocation().y * 32));
		}











		// Assign a destination

		if (n->isIdle() || n->isGatheringMinerals())
		{
				TilePosition  a = PotentialEnnemyBases.at(BaseChecked);
	
			Scouts.move(Position(a.x * 32,a.y * 32));
			Broodwar << "new X : " << a.x << std::endl;
			Broodwar << "new Y : " << a.y << std::endl;

			if (BaseChecked < PotentialEnnemyBases.size() - 1)
			{

				BaseChecked++;
				Broodwar << "new iterator : " << BaseChecked << std::endl;
			}
			else
			{
				BaseChecked = 0;
			}
		}

	}

	


}
























void StrategyManager::onUnitCreate(BWAPI::Unit unit)
{

	for (IAManager* n : Managers)
	{

		n->onUnitCreate(unit);
	}
}

void StrategyManager::onUnitDestroy(BWAPI::Unit unit)
{

	for (IAManager* n : Managers)
	{

		n->onUnitDestroy(unit);
	}
}

void StrategyManager::onUnitDiscover(BWAPI::Unit unit)
{

	for (IAManager* n : Managers)
	{

		n->onUnitDiscover(unit);
	}
}

void StrategyManager::onUnitComplete(BWAPI::Unit unit)
{

	for (IAManager* n : Managers)
	{

		n->onUnitComplete(unit);
	}
}
