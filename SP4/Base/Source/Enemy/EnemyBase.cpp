#include "EnemyBase.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "..\Weapon\WeaponBase.h"
#include "..\State\StateManager.h"

EnemyBase::EnemyBase(Mesh* _modelMesh) :
GenericEntity(_modelMesh),
moveSpeed(10),
maxSpeed(1),
weapon(NULL),
health(0),
waypointIndex(0),
stateManager(NULL),
maxHealth(0)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update(double dt)
{
	EntityBase::Update(dt);

	{//Max speed
		if (!velocity.IsZero())//if zero speed, skip checks
		{
			if (velocity.LengthSquared() > maxSpeed * maxSpeed)
			{
				velocity -= velocity.Normalized() * (moveSpeed*1.1f) * dt;
				if (velocity.LengthSquared() < maxSpeed * maxSpeed)
					velocity = velocity.Normalized() * maxSpeed;
			}
		}
	}

	position += velocity * dt;
	
	if (weapon)
	{
		weapon->SetPosition(this->position);
		weapon->Update(dt);
	}
	{//Update states for AI
		if (stateManager)
		{
			HandleState(dt);
			stateManager->Update(dt);
		}
	}
}

void EnemyBase::MoveTo(Vector3 point, double dt)
{
	Vector3 dir = point - position;
	if (dir.IsZero())
		return;
	
	velocity += dir.Normalized() * moveSpeed * dt;
}

void EnemyBase::MoveTowards(Vector3 dir, double dt)
{
	if (dir.IsZero())
		return;

	velocity += dir.Normalized() * moveSpeed * dt;
}

void EnemyBase::Patrol(double dt)
{
	if (waypoints.size() > 0  && waypointIndex < waypoints.size())
	{
		if (ReachedWaypont())
		{
			velocity.SetZero();
			waypointIndex = (waypointIndex + 1) % waypoints.size();
		}
		else
		{
			Vector3 dir = waypoints[waypointIndex] - this->position;
			if (!dir.IsZero())
			{
				dir.Normalize();
				velocity += dir * moveSpeed * dt;
			}
		}
	}
}

bool EnemyBase::TakeDamage(float amount, EntityBase* other)
{
	this->health -= amount;

	if (health <= 0)
	{
		health = 0;
		b_IsDead = true;

		return true;
	}

	return false;
}

bool EnemyBase::ReachedWaypont(float offset)
{
	float distSquared = (this->position - waypoints[waypointIndex]).LengthSquared();
	float radius = scale.x + offset;

	if (distSquared <= radius * radius)
		return true;
	else
		return false;
}

Vector3 EnemyBase::GetNearestWaypoint()
{
	float closestDist = FLT_MAX;
	int nearestWaypointIndex = 0;

	for (int i = 0; i < waypoints.size(); ++i)
	{
		float distSquared = (waypoints[i] - position).LengthSquared();
		if (distSquared < closestDist)
		{
			closestDist = distSquared;
			nearestWaypointIndex = i;
		}
	}

	return waypoints[nearestWaypointIndex];
}

void EnemyBase::Render()
{
    GenericEntity::Render();
}