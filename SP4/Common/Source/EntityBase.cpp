#include "EntityBase.h"

EntityBase::EntityBase() 
	: position(0.0f, 0.0f, 0.0f)
	, scale(1.0f, 1.0f, 1.0f)
	, b_IsDone(false)
	, b_IsDead(false)
	, b_HasCollider(false)
{
}

EntityBase::~EntityBase()
{
}

void EntityBase::UpdateInputs(double dt)
{
}

void EntityBase::Update(double _dt)
{
	if (this->b_IsDead)
		b_IsDone = true;
}

void EntityBase::Render()
{
}

void EntityBase::RenderUI()
{
}

void EntityBase::HandleOutOfBounds(Vector3 min, Vector3 max, double dt)
{
}

void EntityBase::HandleOutOfBounds(Vector3 point, float radius, double dt)
{
}

void EntityBase::CollisionResponse(EntityBase* other, double dt)
{
}
