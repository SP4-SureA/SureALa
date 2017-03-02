#include "ParticleBase.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

ParticleBase::ParticleBase(Mesh* _modelMesh) :
GenericEntity(_modelMesh),
rotate(0.0f),
rotationSpeed(0.0f),
lifespan(0.0f),
isSprite(false),
isEntered(false),
isExited(false)
{
}

ParticleBase::~ParticleBase()
{
}

void ParticleBase::Update(double dt)
{
	EntityBase::Update(dt);

	if (!isEntered)//if not entered, update Enter
	{//Update enter
		UpdateEnter(dt);
	}
	else if(lifespan > 0.0f)
	{//update particle
		UpdateBehaviour(dt);

		lifespan -= dt;
		if (lifespan < 0.0f)
			lifespan = 0.0f;
	}
	else if (!isExited)//if not exited, update Exit
	{//Update exit
		UpdateExit(dt);
	}
	else
	{
		SetIsDead(true);
	}
}

void ParticleBase::UpdateEnter(double dt)
{
	isEntered = true;
}
void ParticleBase::UpdateBehaviour(double dt)
{
	rotate += rotationSpeed * dt;
	position += velocity * dt;
}
void ParticleBase::UpdateExit(double dt)
{
	isExited = true;
}

void ParticleBase::UpdateAnimation(double dt)
{
	animationPlayer.Update(dt);
}

void ParticleBase::HandleOutOfBounds(Vector3 min, Vector3 max, double dt)
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

void ParticleBase::Render()
{
	if (!b_DoRender)
		return;
	
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Rotate(rotate, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
	if (isSprite)
		RenderHelper::RenderSprite(modelMesh, animationPlayer);
	else
		RenderHelper::RenderMesh(modelMesh);
	modelStack.PopMatrix();
}