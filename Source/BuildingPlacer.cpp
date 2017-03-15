#include "BuildingPlacer.h"

BuildingPlacer::BuildingPlacer()
{
	reserveMap.resize(BWAPI::Broodwar->mapWidth(), BWAPI::Broodwar->mapHeight());
	reserveMap.setTo(false);
	this->buildDistance = 1;
}
bool BuildingPlacer::canBuildHere(BWAPI::TilePosition position_t, BWAPI::UnitType type) const
{
	//returns true if we can build this type of unit here. Takes into account reserved tiles.
	if (!BWAPI::Broodwar->canBuildHere(position_t, type))
		return false;
	for (int x = position_t.x; x < position_t.x + type.tileWidth(); ++x)
		for (int y = position_t.y; y < position_t.y + type.tileHeight(); ++y)
			if (reserveMap[x][y])
				return false;
	return true;
}
bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position_t, BWAPI::UnitType type) const
{
	return canBuildHereWithSpace(position_t, type, this->buildDistance);
}
bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position_t, BWAPI::UnitType type, int buildDist) const
{
	//returns true if we can build this type of unit here with the specified amount of space.
	//space value is stored in this->buildDistance.

	//if we can't build here, we of course can't build here with space
	if (!this->canBuildHere(position_t, type))
		return false;

	int width = type.tileWidth();
	int height = type.tileHeight();

	//make sure we leave space for add-ons. These types of units can have addons:
	if (type.canBuildAddon())
	{
		width += 2;
	}
	int startx = position_t.x - buildDist;
	if (startx < 0) return false;
	int starty = position_t.y - buildDist;
	if (starty < 0) return false;
	int endx = position_t.x + width + buildDist;
	if (endx>BWAPI::Broodwar->mapWidth()) return false;
	if (endx<position_t.x + width) return false;
	int endy = position_t.y + height + buildDist;
	if (endy>BWAPI::Broodwar->mapHeight()) return false;

	for (int x = startx; x < endx; x++)
		for (int y = starty; y < endy; y++)
			if (!type.isRefinery() && (!buildable(x, y) || reserveMap[x][y]))
				return false;

	if (position_t.x > 3)
	{
		int startx2 = startx - 2;
		if (startx2 < 0) startx2 = 0;
		for (int x = startx2; x < startx; ++x)
			for (int y = starty; y < endy; ++y)
			{
				BWAPI::Unitset units = BWAPI::Broodwar->getUnitsOnTile(x, y);
				for (BWAPI::Unitset::iterator i = units.begin(); i != units.end(); ++i)
				{
					if (!(*i)->isLifted() && (*i)->getType().canBuildAddon())
						return false;
				}
			}
	}
	return true;
}
BWAPI::TilePosition BuildingPlacer::getBuildLocation(BWAPI::UnitType type) const
{
	//returns a valid build location if one exists, scans the map left to right
	for (int x = 0; x < BWAPI::Broodwar->mapWidth(); x++)
		for (int y = 0; y < BWAPI::Broodwar->mapHeight(); y++)
			if (this->canBuildHere(BWAPI::TilePosition(x, y), type))
				return BWAPI::TilePosition(x, y);
	return BWAPI::TilePositions::None;
}

BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(BWAPI::TilePosition position_t, BWAPI::UnitType type) const
{
	return getBuildLocationNear(position_t, type, this->buildDistance);
}

BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(BWAPI::TilePosition position_t, BWAPI::UnitType type, int buildDist) const
{
	//returns a valid build location near the specified tile position_t.
	//searches outward in a spiral.
	int x = position_t.x;
	int y = position_t.y;
	int length = 1;
	int j = 0;
	bool first = true;
	int dx = 0;
	int dy = 1;
	while (length < BWAPI::Broodwar->mapWidth()) //We'll ride the spiral to the end
	{
		//if we can build here, return this tile position_t
		if (x >= 0 && x < BWAPI::Broodwar->mapWidth() && y >= 0 && y < BWAPI::Broodwar->mapHeight())
			if (this->canBuildHereWithSpace(BWAPI::TilePosition(x, y), type, buildDist))
				return BWAPI::TilePosition(x, y);

		//otherwise, move to another position_t
		x = x + dx;
		y = y + dy;
		//count how many steps we take in this direction
		j++;
		if (j == length) //if we've reached the end, its time to turn
		{
			//reset step counter
			j = 0;

			//Spiral out. Keep going.
			if (!first)
				length++; //increment step counter if needed

			//first=true for every other turn so we spiral out at the right rate
			first = !first;

			//turn counter clockwise 90 degrees:
			if (dx == 0)
			{
				dx = dy;
				dy = 0;
			}
			else
			{
				dy = -dx;
				dx = 0;
			}
		}
		//Spiral out. Keep going.
	}
	return BWAPI::TilePositions::None;
}

bool BuildingPlacer::buildable(int x, int y) const
{
	//returns true if this tile is currently buildable, takes into account units on tile
	if (!BWAPI::Broodwar->isBuildable(x, y)) return false;
	BWAPI::Unitset units = BWAPI::Broodwar->getUnitsOnTile(x, y);
	for (BWAPI::Unitset::iterator i = units.begin(); i != units.end(); ++i)
		if ((*i)->getType().isBuilding() && !(*i)->isLifted())
			return false;
	return true;
}

void BuildingPlacer::reserveTiles(BWAPI::TilePosition position_t, int width, int height)
{
	for (int x = position_t.x; x < position_t.x + width && x < (int)reserveMap.getWidth(); ++x)
		for (int y = position_t.y; y < position_t.y + height && y < (int)reserveMap.getHeight(); ++y)
			reserveMap[x][y] = true;
}

void BuildingPlacer::freeTiles(BWAPI::TilePosition position_t, int width, int height)
{
	for (int x = position_t.x; x < position_t.x + width && x < (int)reserveMap.getWidth(); ++x)
		for (int y = position_t.y; y < position_t.y + height && y < (int)reserveMap.getHeight(); ++y)
			reserveMap[x][y] = false;
}

void BuildingPlacer::setBuildDistance(int distance)
{
	this->buildDistance = distance;
}
int BuildingPlacer::getBuildDistance() const
{
	return this->buildDistance;
}
bool BuildingPlacer::isReserved(int x, int y) const
{
	if (x < 0 || y < 0 || x >= (int)reserveMap.getWidth() || y >= (int)reserveMap.getHeight())
		return false;
	return reserveMap[x][y];
}