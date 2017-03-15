#include "Fiche.h"


Fiche::Fiche(const Fiche::Type& type_t, const BWAPI::UnitType& unitType,const BWAPI::UnitCommandType& unitCommand) : type(type_t)
{
	unitOrder = unitType;
	actionType = unitCommand;

	maxUnitCount = 0;
}

Fiche::~Fiche()
{
}

Fiche::Type Fiche::getType() const
{
	return type;
}

unsigned int Fiche::getCountdown() const
{
	return countdown;
}

bool Fiche::registerUnit(const BWAPI::Unit& unit)
{
	unsigned int score = 0;
	bool successfullyAdded = false;
	//check if the unit is better than another one already assigned
	if (isUnitAGoodCandidate(unit, score))
	{
		//check if there is still enough places
		if (registeredUnits.size() < maxUnitCount)
		{
			//remove the worst unit, add our unit
			registeredUnits.erase(worstUnit);
		}
		registeredUnits.emplace_back(std::pair<BWAPI::Unit, int>(unit, score));
		successfullyAdded = true;
	}
	return successfullyAdded;
}

bool Fiche::isUnitAGoodCandidate(const BWAPI::Unit& unit, unsigned int& score)
{
	score = getScore(unit);
	bool goodCandidate = false;
	unsigned int best{ 0 }, worst{std::numeric_limits<unsigned int>::max()};
	for (std::list<std::pair<BWAPI::Unit, unsigned int>>::iterator& unit = registeredUnits.begin(); unit != registeredUnits.end(); ++unit)
	{
		if (unit->second < worst)
		{
			worst = unit->second;
			worstUnit = unit;
		}
		if (unit->second > best)
		{
			best = unit->second;
			bestunit = unit;
		}
		if (score > unit->second) goodCandidate = true;
	}

	return goodCandidate;
}

unsigned int Fiche::getScore(const BWAPI::Unit& unit)
{
	unsigned int score { 0 };
	BuildingPlacer builder;

	switch (type)
	{
	case BUILDING:
	{
		BWAPI::TilePosition buildPos = builder.getBuildLocationNear(unit->getTilePosition(), unitOrder);
		bool ok = BWAPI::Broodwar->canBuildHere(buildPos, unitOrder);
	}break;
	case RESEARCH:
	{

	}break;
	case UNIT:
	{
		
	}break;
	case GATHERING:
	{
		score = unit->getDistance(unit->getClosestUnit(BWAPI::Filter::IsMineralField));
	}break;
	case ATTACK:
	{

	}break;
	}
		score = unit->getDistance(unit->getClosestUnit(BWAPI::Filter::IsEnemy));
		if (unit->isAttacking()) score = static_cast<unsigned int>(score*1.5);
	return score;
}