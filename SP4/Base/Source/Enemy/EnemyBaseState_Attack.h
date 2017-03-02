#ifndef STATE_ENEMYBASE_ATTACK_H
#define STATE_ENEMYBASE_ATTACK_H

#include "..\State\State.h"
#include "SingletonTemplate.h"

#include "EnemyBase.h"

class EnemyBaseState_Attack : public Singleton<EnemyBaseState_Attack>, public State
{
	friend Singleton<EnemyBaseState_Attack>;
public:
	EnemyBaseState_Attack(){}
	virtual ~EnemyBaseState_Attack(){}

	virtual void OnEnter(EntityBase* entity){}
	virtual void Update(double dt, EntityBase* entity)
	{
		EnemyBase* enemyBase = dynamic_cast<EnemyBase*>(entity);
		if (!enemyBase)
			return;

		enemyBase->Attack();
	}
	virtual void OnExit(EntityBase* entity){}
};

#endif // _EnemyBaseState_Attack_H