#ifndef _PARTICLE_SCALE_H
#define _PARTICLE_SCALE_H

#include "ParticleBase.h"

class ParticleScale : public ParticleBase
{
public:
	ParticleScale(Mesh* _modelMesh);
	virtual ~ParticleScale();

	virtual void UpdateEnter(double dt);
	virtual void UpdateBehaviour(double dt);
	virtual void UpdateExit(double dt);

	void SetupParticle(float _scaleInTimer, float _scaleOutTimer);

	inline void SetDesiredSize(Vector3 size){ this->desiredSize = size; }
	inline Vector3 GetDesiredSize(){ return this->desiredSize; }

	inline void SetMinSize(Vector3 size){ this->minSize = size; }
	inline Vector3 GetMinSize(){ return this->minSize; }

protected:
	Vector3 desiredSize;
	Vector3 minSize;

	float scaleInSpeed;
	float scaleOutSpeed;
};

namespace Create
{
	ParticleScale* particleScale(EntityManager* em,
		const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scale,
		const float& _scaleInTimer,
		const float& _lifespan = 0.0f,
		const float& _scaleOutTimer = 0.0f,
		const Vector3& _minSize = Vector3(0.01f, 0.01f, 0.01f));

	ParticleScale* particleScaleSprite(EntityManager* em,
		const std::string& _meshName,
		const std::string& _animation,
		const Vector3& _position,
		const Vector3& _scale,
		const float& _scaleInTimer,
		const float& _lifespan = 0.0f,
		const float& _scaleOutTimer = 0.0f,
		const Vector3& _minSize = Vector3(0.01f, 0.01f, 0.01f));
};

#endif // _PARTICLE_BASE_H