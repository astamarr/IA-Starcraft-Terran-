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
	Unitset workers, fighters, buildings;
	void update(const int& framesCount);
	void addFiche(std::shared_ptr<Fiche> fiche);
	void removeFiche(std::shared_ptr<Fiche> fiche);
	std::vector<IAManager*> Managers;

	

	// Scouting Data
	std::deque<TilePosition>  PotentialEnnemyBases;
	std::list<TilePosition>  CheckedPositions;
	TilePosition EnnemyBase;
	Unitset Scouts;
	int BaseChecked = 0;





















	 void onStart();
	 void onEnd(bool isWinner);
	 void onFrame();
	 void onSendText(std::string text);
	 void onReceiveText(BWAPI::Player player, std::string text);
	 void onPlayerLeft(BWAPI::Player player);
	 void onNukeDetect(BWAPI::Position target){};
	 void onUnitDiscover(BWAPI::Unit unit);
	 void onUnitEvade(BWAPI::Unit unit){};
	 void onUnitShow(BWAPI::Unit unit){};
	 void onUnitHide(BWAPI::Unit unit){};
	 void onUnitCreate(BWAPI::Unit unit);
	 void onUnitDestroy(BWAPI::Unit unit);
	 void onUnitMorph(BWAPI::Unit unit){};
	 void onUnitRenegade(BWAPI::Unit unit){};
	 void onSaveGame(std::string gameName);
	 void onUnitComplete(BWAPI::Unit unit) ;


	
private:
	IAManager* EconomieManager;
	IAManager* fightsManager;
	IAManager* gathererManager;

	std::vector<std::shared_ptr<Fiche>> fiches;


	void Scouting();
};

