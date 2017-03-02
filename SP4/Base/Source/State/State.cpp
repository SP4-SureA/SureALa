#include "State.h"

State::State()
{
}

State::~State()
{
}

void State::OnEnter(EntityBase* entity)
{
}

void State::Update(double dt, EntityBase* entity)
{
	if (!entity)
		return;
}

void State::OnExit(EntityBase* entity)
{
}