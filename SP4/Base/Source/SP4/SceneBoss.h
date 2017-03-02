#ifndef SCENE_BOSS_H
#define SCENE_BOSS_H

#include "..\SceneBase2D.h"
#include "..\GenericEntity.h"

#include <vector>
using std::vector;

#include <map>
using std::map;

class PlayerInfo;
class SpriteEntity;
class SkyWhale;
class HealthBarRenderer;

class SceneBoss : public SceneBase2D
{
public:
	SceneBoss();
	virtual ~SceneBoss();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
    float currentBossHeath;
	bool pauseGame;

	SkyWhale* bossEntity;
    HealthBarRenderer* healthBar;
	std::map<EntityBase*, SpriteEntity*> reflectionMap;

};

#endif