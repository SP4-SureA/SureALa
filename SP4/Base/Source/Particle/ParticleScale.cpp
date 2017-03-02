#include "ParticleScale.h"

#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

ParticleScale::ParticleScale(Mesh* _modelMesh):
ParticleBase(_modelMesh),
scaleInSpeed(0.0f),
scaleOutSpeed(0.0f)
{
}

ParticleScale::~ParticleScale()
{
}

void ParticleScale::UpdateEnter(double dt)
{
	scale.x += scaleInSpeed * dt;
	scale.y += scaleInSpeed * dt;

	if (scale.x > desiredSize.x)
	{
		scale = desiredSize;
		isEntered = true;
	}
}
void ParticleScale::UpdateBehaviour(double dt)
{
	ParticleBase::UpdateBehaviour(dt);
}
void ParticleScale::UpdateExit(double dt)
{
	scale.x -= scaleOutSpeed * dt;
	scale.y -= scaleOutSpeed * dt;

	if (scale.x < 0.01f)
	{
		scale.Set(0.01f, 0.01f, 0.01f);
		isExited = true;
	}
}

void ParticleScale::SetupParticle(float _scaleInTimer, float _scaleOutTimer)
{
	desiredSize.z = 0.0f;
	minSize.z = 0.0f;
	
	if (_scaleInTimer <= 0.0f)
	{
		isEntered = true;
		scale = desiredSize;
	}
	else
		scaleInSpeed = (desiredSize.x - scale.x) / _scaleInTimer;

	if (_scaleOutTimer <= 0.0f)
		isExited = true;
	else
		scaleOutSpeed = (desiredSize.x - minSize.x) / _scaleOutTimer;
}

ParticleScale* Create::particleScale(EntityManager* em,
	const std::string& _meshName,
	const Vector3& _position,
	const Vector3& _scale,
	const float& _scaleInTimer,
	const float& _lifespan,
	const float& _scaleOutTimer,
	const Vector3& _minSize)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	ParticleScale* result = new ParticleScale(modelMesh);
	result->SetPosition(_position);
	result->SetDesiredSize(_scale);
	result->SetLifespan(_lifespan);
	result->SetScale(_minSize);
	result->SetupParticle(_scaleInTimer, _scaleOutTimer);

	em->AddEntity(result);
	return result;
}

ParticleScale* Create::particleScaleSprite(EntityManager* em,
	const std::string& _meshName,
	const std::string& _animation,
	const Vector3& _position,
	const Vector3& _scale,
	const float& _scaleInTimer,
	const float& _lifespan,
	const float& _scaleOutTimer,
	const Vector3& _minSize
	)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;
	
	ParticleScale* result = new ParticleScale(modelMesh);
	result->SetIsSprite(true);
	result->SetAnimation(_meshName, _animation);
	result->SetPosition(_position);
	result->SetDesiredSize(_scale);
	result->SetLifespan(_lifespan);
	result->SetScale(_minSize);
	result->SetupParticle(_scaleInTimer, _scaleOutTimer);
	
	em->AddEntity(result);
	return result;
}