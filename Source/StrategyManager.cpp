#include "StrategyManager.h"


StrategyManager::StrategyManager()
{
	//décisions de construction, research, etc..
	ressourcesManager = new EconomieIA();
	//décision de combat, production d'unité, upgarde, ...
	fightsManager = new CombatIA();
	//décisions de planification de récoltes 
	gathererManager = new RecolteIA();
}

StrategyManager::~StrategyManager()
{
	delete ressourcesManager;
	delete fightsManager;
	delete gathererManager;
}

void StrategyManager::update(const int& framesCount)
{
	//int framerate = BWAPI::Broodwar->getAverageFPS();
	if (framesCount % 5 == 0) fightsManager->update();
	if (framesCount % 10 == 0) gathererManager->update();
	if (framesCount % 30 == 0) ressourcesManager->update();
}