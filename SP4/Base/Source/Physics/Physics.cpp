#include "Physics.h"

#include "..\GenericEntity.h"

Physics::Physics()
{
}

Physics::~Physics()
{
}

bool Physics::CheckCollision(GenericEntity* a, GenericEntity* b, double dt)
{
	if (!a || !b)
		return false;

	float distancesquared = ((a->GetPosition() + a->GetVelocity() * dt) - (b->GetPosition() + b->GetVelocity() * dt)).LengthSquared();
	float combinedradius = a->GetScale().x + b->GetScale().x;

	//distance check and direction check
	if (distancesquared <= combinedradius * combinedradius)// && servervel.dot(position - point) > 0)
		return true;
	else
		return false;
}

void Physics::CollisionResponse(GenericEntity* a, GenericEntity* b)
{
	if (!a || !b)
		return;
	Vector3 N = (a->GetPosition() - b->GetPosition()).Normalized();

	Vector3 u1N = a->GetVelocity().Dot(N) * N;
	Vector3 u2N = b->GetVelocity().Dot(N) * N;
	float combinedRadius = a->GetScale().x + b->GetScale().x;

	a->SetVelocity(a->GetVelocity() + (((2 * 1) / (1 + 1)) * (u2N - u1N)));
	b->SetVelocity(b->GetVelocity() + (((2 * 1) / (1 + 1)) * (u1N - u2N)));

	//a->SetVelocity(a->GetVelocity() + (((2 * b->mass) / (a->mass + b->mass)) * (u2N - u1N)));
	//b->SetVelocity(b->GetVelocity() + (((2 * a->mass) / (a->mass + b->mass)) * (u1N - u2N)));

	//a->pos = b->pos + (a->pos - b->pos).Normalized() * combinedRadius;
}

bool Physics::CheckCollision2(GenericEntity* a, GenericEntity* b, double dt)
{
	if (!a || !b)
		return false;
}

void Physics::CollisionResponse2(GenericEntity* a, GenericEntity* b)
{
	if (!a || !b)
		return;
}