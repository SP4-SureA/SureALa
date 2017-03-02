#ifndef _SPAWN_STATE_H
#define _SPAWN_STATE_H

#include "../../State/State.h"

#include <vector>
using std::vector;

class EntityBase;
class BossEntity;

class SpawnState : public State
{
public:
	SpawnState();
	virtual ~SpawnState();

	virtual void OnEnter(EntityBase* entity);
	virtual void Update(double dt, EntityBase* entity);
	virtual void OnExit(EntityBase* entity);

	void SpawnBossMinions(double dt, BossEntity* entity);
	void UpdateGraveVector(double dt);
	bool GetIfAllGravesAreDead(){ return this->p_graveEntityVector.empty(); };
	
    inline int GetNumberOfGravesLeft(){ return this->p_graveEntityVector.size(); };
    inline int GetMaxNumberOfGraves(){ return this->maxNumberOfGraves; };
    
protected :
    float maxNumberOfGraves;
    float countdownTime;
	vector<EntityBase*> p_graveEntityVector;

};

#endif // _SPAWN_STATE_H