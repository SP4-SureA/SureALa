#include "Physics.h"

#include "EntityBase.h"

Physics::Physics()
{
}

Physics::~Physics()
{
}

bool Physics::CheckCollision(EntityBase* a, EntityBase* b, double dt)
{
	if (!a || !b)
		return false;

	EntityBase *goA = a;
	EntityBase *goB = b;
	if (a->GetColliderType() != Collider::COLLIDER_SPHERE)
	{
		if (b->GetColliderType() != Collider::COLLIDER_SPHERE)
			return false;
		
		goA = b;
		goB = a;
	}

	switch (goB->GetColliderType())
	{
	case Collider::COLLIDER_SPHERE:
		return CheckSphereSphere(goA, goB, dt);
		break;
	case Collider::COLLIDER_BOX:
		return CheckSphereBox(goA, goB, dt);
		break;

	default:return false; break;
	}
}

void Physics::CollisionResponse(EntityBase* a, EntityBase* b)
{
	if (!a || !b)
		return;

	EntityBase *goA = a;
	EntityBase *goB = b;
	if (a->GetColliderType() != Collider::COLLIDER_SPHERE)
	{
		if (b->GetColliderType() != Collider::COLLIDER_SPHERE)
			return;

		goA = b;
		goB = a;
	}

	switch (goB->GetColliderType())
	{
	case Collider::COLLIDER_SPHERE:
		ResponseSphereSphere(goA, goB);
		break;
	case Collider::COLLIDER_BOX:
		ResponseSphereBox(goA, goB);
		break;

	default:return; break;
	}
}

bool Physics::CheckSphereSphere(EntityBase* a, EntityBase* b, double dt)
{
	Vector3 aForwardPos = ((a->GetPosition() + a->GetColliderOffset()) + a->GetVelocity() * dt);
	Vector3 bForwardPos = ((b->GetPosition() + b->GetColliderOffset()) + b->GetVelocity() * dt);
	float distancesquared = (aForwardPos - bForwardPos).LengthSquared();

	float combinedradius = a->GetColliderSize().x + b->GetColliderSize().x;

	//distance check and direction check
	if (distancesquared <= combinedradius * combinedradius)// && servervel.dot(position - point) > 0)
		return true;
	else
		return false;
}
bool Physics::CheckSphereBox(EntityBase* a, EntityBase* b, double dt)
{
	Vector3 aPos = (a->GetPosition() + a->GetColliderOffset());
	Vector3 bPos = (b->GetPosition() + b->GetColliderOffset());

	Vector3 N = b->GetFront();
	Vector3 NP = Vector3(N.y, -N.x, N.z);
	Vector3 goPos = aPos + a->GetVelocity() * dt;
	Vector3 go2Pos = bPos;
	Vector3 dir = go2Pos - goPos;

	return abs(dir.Dot(N)) < a->GetColliderSize().x + b->GetColliderSize().x * 0.5f
		&&
		abs(dir.Dot(NP)) < a->GetColliderSize().x + b->GetColliderSize().y * 0.5f;
}

void Physics::ResponseSphereSphere(EntityBase* a, EntityBase* b)
{
	Vector3 N = (a->GetPosition() + a->GetColliderOffset()) - (b->GetPosition() + b->GetColliderOffset());

	if (N.IsZero())
		return;
	else
		N.Normalize();

	Vector3 u1N = a->GetVelocity().Dot(N) * N;
	Vector3 u2N = b->GetVelocity().Dot(N) * N;
	float distanceSquared = ((a->GetPosition() + a->GetColliderOffset()) - (b->GetPosition() + b->GetColliderOffset())).LengthSquared();
	float combinedRadius = a->GetColliderSize().x + b->GetColliderSize().x;
	float difference = distanceSquared - (combinedRadius * combinedRadius);

	a->AddVelocity(((2 * b->GetWeight()) / (a->GetWeight() + b->GetWeight())) * (u2N - u1N));
	a->AddVelocity(-N * difference);
	b->AddVelocity(((2 * a->GetWeight()) / (a->GetWeight() + b->GetWeight())) * (u1N - u2N));
	b->AddVelocity(N * difference);
}

void Physics::ResponseSphereBox(EntityBase* a, EntityBase* b)
{
	Vector3 N = b->GetFront();
	Vector3 aPos = a->GetPosition() + a->GetColliderOffset();
	Vector3 bPos = b->GetPosition() + b->GetColliderOffset();
	Vector3 ab = bPos - aPos;

	if (abs(ab.Dot(N)) < b->GetColliderSize().x * 0.5f)
	{
		Vector3 NP = Vector3(-N.y, N.x, N.z);
		if (ab.Dot(NP) < 0)
			NP = -NP;
		if (a->GetVelocity().Dot(NP) < 0)
			return;

		a->AddVelocity(-((2 * a->GetVelocity()).Dot(NP)) * NP);

		float difference = (b->GetColliderSize().y * 0.5f + a->GetColliderSize().x) - ab.Dot(NP);
		a->SetPosition(a->GetPosition() -NP * difference);
	}
	else
	{
		if (ab.Dot(N) < 0)
			N = -N;
		if (a->GetVelocity().Dot(N) < 0)
			return;

		a->AddVelocity(-((2 * a->GetVelocity()).Dot(N)) * N);

		float difference = (b->GetColliderSize().x * 0.5f + a->GetColliderSize().x) - ab.Dot(N);
		a->SetPosition(a->GetPosition() - N * difference);
	}
}