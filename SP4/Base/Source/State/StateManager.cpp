#include "StateManager.h"

StateManager::StateManager() :
b_shouldChange(false),
attachedEntity(nullptr),
currentState(nullptr)
{
}

StateManager::~StateManager()
{
}

void StateManager::Update(double dt)
{
	if (this->currentState)
		currentState->Update(dt, attachedEntity);
}

EntityBase* StateManager::DetachEntity()
{
	EntityBase* temp = attachedEntity;
	attachedEntity = nullptr;
	return temp;
}

void StateManager::SetCurrentState(State* _state)
{ 
	if (this->currentState)
		this->currentState->OnExit(attachedEntity);

	this->currentState = _state; 

	if (this->currentState)
		this->currentState->OnEnter(attachedEntity);
}
