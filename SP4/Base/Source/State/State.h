#ifndef _STATE_H
#define _STATE_H

#include "EntityBase.h"

class State
{
public:
	State();
	virtual ~State();

	virtual void OnEnter(EntityBase* entity);
	virtual void Update(double dt, EntityBase* entity);
	virtual void OnExit(EntityBase* entity);
};

#endif // _STATE_H