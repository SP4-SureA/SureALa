#ifndef FLOORTILES_MANAGER_H
#define FLOORTILES_MANAGER_H

#include "SingletonTemplate.h"

#include <list>
using std::list;

class FloorTile;

class FloorTilesManager : public Singleton<FloorTilesManager>
{
	friend Singleton<FloorTilesManager>;
public:
	virtual ~FloorTilesManager();

	bool AddFloor(FloorTile* flr);
	bool RemoveFloor(FloorTile* flr);

	inline list<FloorTile*> GetList(){ return this->floorTileList; }

protected:
	FloorTilesManager();

	list<FloorTile*> floorTileList;

};

#endif