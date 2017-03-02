#ifndef _STATE_MANAGER_H
#define _STATE_MANAGER_H

#include "State.h"

class EntityBase;

class StateManager  
{
public:
	StateManager();
	virtual ~StateManager();

    virtual void Init(){};
	virtual void Exit(){};

	virtual void Update(double dt);

	void AttachEntity(EntityBase* _entity){ this->attachedEntity = _entity; };
	EntityBase* DetachEntity();

	inline void SetShouldChange(bool _ShouldChange){this->b_shouldChange = _ShouldChange; };
	inline bool GetShouldChange(){ return this->b_shouldChange; };

	inline State* GetCurrentState(){ return this->currentState; };
	void SetCurrentState(State* _state);

protected:
	State* currentState;

	EntityBase* attachedEntity;
	bool b_shouldChange;

};

#endif // _STATE_MANAGER_H