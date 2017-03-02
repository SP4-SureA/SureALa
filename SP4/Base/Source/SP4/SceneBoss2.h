#ifndef SCENE_BOSS2_H
#define SCENE_BOSS2_H

#include "..\SceneBase2D.h"
#include "..\GenericEntity.h"

#include <vector>
using std::vector;

#include <map>
using std::map;

class PlayerInfo;
class SpriteEntity;
class FloorTile;

class SceneBoss2 : public SceneBase2D
{
public:
	SceneBoss2();
	virtual ~SceneBoss2();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	std::map<PlayerInfo*, SpriteEntity*> reflectionMap;
	FloorTile* floor;
};

#endif