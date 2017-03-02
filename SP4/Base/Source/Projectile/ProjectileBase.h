#ifndef _PROJECTILE_BASE_H
#define _PROJECTILE_BASE_H

#include "Vector3.h"
#include "../GenericEntity.h"
#include "AnimationManager.h"
#include "Animation.h"

class ProjectileBase : public GenericEntity
{
public:
    virtual ~ProjectileBase();

    virtual void Update(double dt);
	virtual void UpdateAnimation(double dt);
	virtual void HandleOutOfBounds(Vector3 min, Vector3 max, double dt);
    virtual void Render();

    inline void SetProjectileDamage(const float& _damage){ this->projectile_Damage = _damage; };
    inline float GetProjectileDamage(void){ return this->projectile_Damage; };

	inline void SetLifespan(const float& _lifespan){ this->lifespan = _lifespan; };
	inline float GetLifespan(void){ return this->lifespan; }

	virtual void UpdateLifeSpan(double dt);

	inline void SetAnimation(std::string _meshName, std::string _animationName){ animationPlayer.m_anim = AnimationManager::GetInstance(_meshName)->GetAnimation(_animationName); }
	inline AnimationPlayer* GetAnimationPlayer(){ return &this->animationPlayer; }

protected:
	ProjectileBase(Mesh* _modelMesh);

    float projectile_Damage;

	float lifespan;
	float currentLifetime;

	AnimationPlayer animationPlayer;
};


#endif // _PROJECTILE_BASE_H