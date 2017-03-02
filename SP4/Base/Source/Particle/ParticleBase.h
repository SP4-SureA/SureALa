#ifndef _PARTICLE_BASE_H
#define _PARTICLE_BASE_H

#include "Vector3.h"
#include "../GenericEntity.h"
#include "AnimationManager.h"
#include "Animation.h"

class ParticleBase : public GenericEntity
{
public:
    virtual ~ParticleBase();

    virtual void Update(double dt);
	virtual void UpdateAnimation(double dt);
	virtual void HandleOutOfBounds(Vector3 min, Vector3 max, double dt);
    virtual void Render();

	virtual void UpdateEnter(double dt);
	virtual void UpdateBehaviour(double dt);
	virtual void UpdateExit(double dt);

	inline void SetIsSprite(bool _bool){ this->isSprite = _bool; }

	inline void SetLifespan(const float& _lifespan){ this->lifespan = _lifespan; };
	inline float GetLifespan(void){ return this->lifespan; }

	inline void SetRotateSpeed(float speed){ this->rotationSpeed = speed; }
	inline float GetRotateSpeed(){ return this->rotationSpeed; }

	inline void SetRotate(float angle){ this->rotate = angle; }
	inline float GetRotate(){ return this->rotate; }

	inline void SetAnimation(std::string _meshName, std::string _animationName){ animationPlayer.m_anim = AnimationManager::GetInstance(_meshName)->GetAnimation(_animationName); }
	inline AnimationPlayer* GetAnimationPlayer(){ return &this->animationPlayer; }

protected:
	ParticleBase(Mesh* _modelMesh);
	bool isSprite;

	float lifespan;
	
	float rotate;
	float rotationSpeed;

	bool isEntered;
	bool isExited;

	AnimationPlayer animationPlayer;
};

#endif // _PARTICLE_BASE_H