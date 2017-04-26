#include "StrategyManager.h"

using namespace BWAPI;
using namespace Filter;

StrategyManager::StrategyManager()
{
	//décisions de construction, research, etc..
	EconomieManager = new EconomieIA();
	//décision de combat, production d'unité, upgarde, ...
	fightsManager = new CombatIA();
	//décisions de planification de récoltes 
	gathererManager = new RecolteIA();
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
	Unitset workers, fighters, buildings;

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
		
		if (u->getType().isWorker())
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
}