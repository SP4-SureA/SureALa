#ifndef _ANGRY_STATE_H
#define _ANGRY_STATE_H

#include "../../State/State.h"

#include <vector>
using std::vector;

class EntityBase;
class BossEntity;

class AngryState : public State
{
public:
	AngryState();
	virtual ~AngryState();

	virtual void OnEnter(EntityBase* entity);
	virtual void Update(double dt, EntityBase* entity);
	virtual void OnExit(EntityBase* entity);

    void UpdateAction(double dt, EntityBase* entity);
    void DoAction(double dt, EntityBase* entity);

	void SpawnBossMinions(double dt, EntityBase* entity);

protected :
	float currActionCooldownTime;
    float actionCooldown;
};

#endif // _ANGRY_STATE_H