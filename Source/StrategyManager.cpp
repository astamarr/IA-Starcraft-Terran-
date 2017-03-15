#include "StrategyManager.h"


StrategyManager::StrategyManager()
{
	//d�cisions de construction, research, etc..
	ressourcesManager = new EconomieIA();
	//d�cision de combat, production d'unit�, upgarde, ...
	fightsManager = new CombatIA();
	//d�cisions de planification de r�coltes 
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