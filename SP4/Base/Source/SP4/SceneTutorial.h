#ifndef SCENE_TUTORIAL_H
#define SCENE_TUTORIAL_H

#include "..\SceneBase2D.h"
#include "..\GenericEntity.h"

#include <vector>
using std::vector;

#include <map>
using std::map;

class PlayerInfo;
class SpriteEntity;
class FloorTile;

class SceneTutorial : public SceneBase2D
{
public:
	SceneTutorial();
	virtual ~SceneTutorial();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	bool enemiesSpawned;
	bool pauseGame;
	GenericEntity* goal;
	GenericEntity* spawnTrigger;
	FloorTile* floor;
};

#endif