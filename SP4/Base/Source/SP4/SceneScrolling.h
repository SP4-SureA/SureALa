#ifndef SCENE_SCROLLING_H
#define SCENE_SCROLLING_H

#include "..\SceneBase2D.h"
#include "..\GenericEntity.h"

#include <vector>
using std::vector;

class EnemyBase;
class CloudFactory;
class ScrollingEntity;
class FloorTile;
class HealthBarRenderer;

class SceneScrolling : public SceneBase2D
{
public:
	SceneScrolling();
	virtual ~SceneScrolling();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

    void WavesInit();

protected:
	bool pauseGame;

	float flapflapOffsetSpeed;
	float flapflapXoffset;
	float flapflapOffsetLimit;
	int flapflapDirection;

	float scrollSpeed;
	float yDestination;
	bool cutSceneFinished;

	GenericEntity* groundEntity;
	GenericEntity* bossDoorEntity;
	CloudFactory* cloudFactory;
	ScrollingEntity* cloudScrollUI[2];
	std::vector<FloorTile*> floorList;
	FloorTile* bossDoorEntrance;
	
	std::vector<std::vector<EnemyBase*>> waveList;
	int waveIndex;
	bool waveCleared;  

    int numberOfWaves = 0;
    HealthBarRenderer* progressBar;
	float currentProgress;
};

#endif