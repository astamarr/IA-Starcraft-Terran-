#pragma once

#include "IAManager.h"

#include "EconomieIA.h"
#include "CombatIA.h"
#include "RecolteIA.h"

class StrategyManager
{
public:
	StrategyManager();
	~StrategyManager();

	void update(const int& framesCount);
	void addFiche(std::shared_ptr<Fiche> fiche);
	void removeFiche(std::shared_ptr<Fiche> fiche);

private:
	IAManager* EconomieManager;
	IAManager* fightsManager;
	IAManager* gathererManager;

	std::vector<std::shared_ptr<Fiche>> fiches;
};

