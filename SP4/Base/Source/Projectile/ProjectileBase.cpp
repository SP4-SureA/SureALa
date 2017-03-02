#include "ProjectileBase.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

ProjectileBase::ProjectileBase(Mesh* _modelMesh) :
GenericEntity(_modelMesh),
projectile_Damage(0),
lifespan(0),
currentLifetime(0)
{
}

ProjectileBase::~ProjectileBase()
{
}

void ProjectileBase::Update(double dt)
{
	EntityBase::Update(dt);

	this->position += velocity * dt;

	UpdateLifeSpan(dt);
}

void ProjectileBase::UpdateAnimation(double dt)
{
	animationPlayer.Update(dt);
}

void ProjectileBase::UpdateLifeSpan(double dt)
{
	if (currentLifetime > lifespan)
	{
		this->b_IsDead = true;
	}

	currentLifetime += dt;
}

void ProjectileBase::HandleOutOfBounds(Vector3 min, Vector3 max, double dt)
{
	if (position.x < min.x || position.x > max.x)
	{
		SetIsDead(true);
	}
	if (position.y < min.y || position.y > max.y)
	{
		SetIsDead(true);
	}
}

void ProjectileBase::Render()
{
    GenericEntity::Render();
}