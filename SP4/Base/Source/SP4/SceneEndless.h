#ifndef _SCENE_ENDLESS_H
#define _SCENE_ENDLESS_H

#include "..\SceneBase2D.h"
#include "..\GenericEntity.h"

#include <vector>
using std::vector;

class EnemyBase;
class CloudFactory;
class ScrollingEntity;
class FloorTile;
class HealthBarRenderer;

class SceneEndless : public SceneBase2D
{
public:
	SceneEndless();
	virtual ~SceneEndless();

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

	GenericEntity* groundEntity;
	CloudFactory* cloudFactory;
	ScrollingEntity* cloudScrollUI[2];
	std::vector<FloorTile*> floorList;

    // Enemy Wave
	std::vector<std::vector<EnemyBase*>> waveList;
	int waveIndex;
	bool waveCleared;  

    // For Progress Bar
    int numberOfWaves = 0;
    HealthBarRenderer* progressBar;
	float currentProgress;

    // Endless Variables 
    int currentLevel;
    float stageDelay;
};

#endif //_SCENE_ENDLESS_H