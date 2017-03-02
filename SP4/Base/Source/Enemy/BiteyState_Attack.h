#ifndef STATE_BITEY_ATTACK_H
#define STATE_BITEY_ATTACK_H

#include "..\State\State.h"
#include "SingletonTemplate.h"

#include "Bitey.h"

class BiteyState_Attack : public Singleton<BiteyState_Attack>, public State
{
	friend Singleton<BiteyState_Attack>;
public:
	BiteyState_Attack(){}
	virtual ~BiteyState_Attack(){}

	virtual void OnEnter(EntityBase* entity){}
	virtual void Update(double dt, EntityBase* entity)
	{
		//Bitey* bitey = dynamic_cast<Bitey*>(entity);
		//if (!bitey)
		//	return;
	}
	virtual void OnExit(EntityBase* entity){}
};

#endif // _BiteyState_Attack_H