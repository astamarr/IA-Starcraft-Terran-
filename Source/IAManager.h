#pragma once

#include "Fiche.h"
#include <BWAPI.h>

#include <vector>
#include <memory>

class IAManager
{
public:
	void assignUnits(BWAPI::Unitset units);
	virtual void update() = 0;



	virtual void onStart(){};
	virtual void onEnd(bool isWinner){}
	virtual void onFrame(){};
	virtual void onSendText(std::string text){};
	virtual void onReceiveText(BWAPI::Player player, std::string text){};
	virtual void onPlayerLeft(BWAPI::Player player){};
	virtual void onNukeDetect(BWAPI::Position target){};
	virtual void onUnitDiscover(BWAPI::Unit unit){};
	virtual void onUnitEvade(BWAPI::Unit unit){};
	virtual void onUnitShow(BWAPI::Unit unit){};
	virtual void onUnitHide(BWAPI::Unit unit){};
	virtual void onUnitCreate(BWAPI::Unit unit){};
	virtual void onUnitDestroy(BWAPI::Unit unit){};
	virtual void onUnitMorph(BWAPI::Unit unit){};
	virtual void onUnitRenegade(BWAPI::Unit unit){};
	virtual void onSaveGame(std::string gameName){};
	virtual void onUnitComplete(BWAPI::Unit unit){};


protected:
	virtual Fiche createFiche() = 0;
	unsigned int CurrentFrame;
	void findBestPosition(const BWAPI::Unit&);
	std::vector<std::shared_ptr<Fiche>> fiches;
	BWAPI::Unitset assignedUnits;

	Fiche* defaultBehaviour;
};

