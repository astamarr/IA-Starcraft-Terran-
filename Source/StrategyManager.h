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

	void update();
	//bool addFiche(std::shared_ptr<Fiche> fiche);

private:
	IAManager* ressourcesManager;
	IAManager* fightsManager;
	IAManager* gathererManager;

	std::vector<std::shared_ptr<Fiche>> fiches;
};

