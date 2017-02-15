#include "ProjectileBase.h"
#include "Bitstream.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

ProjectileBase::ProjectileBase(Mesh* _modelMesh) :
NetworkEntity(_modelMesh),
projectile_Damage(0)
{
}

ProjectileBase::~ProjectileBase()
{
}

void ProjectileBase::Update(double dt)
{
    this->position += velocity * dt;
}

void ProjectileBase::Render()
{
    GenericEntity::Render();
}