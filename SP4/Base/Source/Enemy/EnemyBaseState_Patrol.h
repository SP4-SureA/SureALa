#ifndef STATE_ENEMYBASE_PATROL_H
#define STATE_ENEMYBASE_PATROL_H

#include "..\State\State.h"
#include "SingletonTemplate.h"

#include "EnemyBase.h"

class EnemyBaseState_Patrol : public Singleton<EnemyBaseState_Patrol>, public State
{
	friend Singleton<EnemyBaseState_Patrol>;
public:
	EnemyBaseState_Patrol(){}
	virtual ~EnemyBaseState_Patrol(){}

	virtual void OnEnter(EntityBase* entity){}
	virtual void Update(double dt, EntityBase* entity)
	{
		EnemyBase* enemyBase = dynamic_cast<EnemyBase*>(entity);
		if (!enemyBase)
			return;

		enemyBase->Patrol(dt);
	}
	virtual void OnExit(EntityBase* entity){}
};

#endif // _EnemyBaseState_Patrol_H