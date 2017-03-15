#include "IAManager.h"

void IAManager::assignUnits(BWAPI::Unitset set)
{
	assignedUnits = set;
}

void IAManager::findBestPosition(const BWAPI::Unit& unit)
{
	std::pair<unsigned int, std::shared_ptr<Fiche>> bestFiche(0, nullptr);
	unsigned int bestScore = 0;
	for (std::vector<std::shared_ptr<Fiche>>::iterator& fiche = fiches.begin(); fiche != fiches.end(); ++fiche)
	{
		unsigned int score = 0;
		Fiche* f = fiche->get();
		if (f->isUnitAGoodCandidate(unit, score))
		{
			if (score >= bestScore)
			{
				bestScore = score;
				bestFiche = std::pair<unsigned int, std::shared_ptr<Fiche>>(score, (*fiche));
			}
		}
	}
	//if there is a task for our man
	if (bestFiche.second != nullptr && bestFiche.second.get()->registerUnit(unit))
	{ 
		//all good 
	}
	else
		defaultBehaviour->registerUnit(unit);
}