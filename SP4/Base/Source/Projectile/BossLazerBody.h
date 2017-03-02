#ifndef _BOSS_LAZER_BODY_H
#define _BOSS_LAZER_BODY_H

#include "Vector3.h"
#include "ProjectileBase.h"

class PlayerEntityBase;

class BossLazerBody : public ProjectileBase
{
public:
    BossLazerBody(Mesh* _modelMesh);
    virtual ~BossLazerBody();

    virtual void Update(double dt);
    virtual void Render();

    void LazerUpdate(double dt);

    inline Vector3 GetStartPosition(){ return this->startPosition; };
    inline void SetStartPosition(Vector3 _startPosition){ this->startPosition = _startPosition; };

    inline float GetLazerLength(){ return this->lazerLength; };
    inline void SetLazerLength(float _length){ this->lazerLength = _length; };

    inline float GetGrowTime(){ return this->growTime; };
    inline void SetGrowTime(float _growTime){ this->growTime = _growTime; };

    inline float GetCurrentGrowTime(){ return this->currentGrowTime; };
    inline void SetCurrentGrowTime(float _growTime){ this->currentGrowTime = _growTime; };

    inline Vector3 GetLazerScale(){ return this->lazerScale; };
    inline void SetLazerScale(Vector3 _lazerScale){ this->lazerScale = _lazerScale; };

	virtual void HandleOutOfBounds(Vector3 min, Vector3 max, double dt);

	virtual void CollisionResponse(EntityBase* other, double dt);

protected:
	bool b_isCollided;

    float growTime; 
    float lazerLength;
    float currentGrowTime;
    Vector3 lazerScale;
    Vector3 startPosition;
   
    PlayerEntityBase* collidedPlayer;
};

namespace Create
{
    BossLazerBody* bossLazerBodyEntity(
        EntityManager* em,
        const std::string& _meshName,
        const std::string& _animationName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const Vector3& _startPosition = Vector3(0.f, 0.f, 0.f),
        const Vector3& _direction = Vector3(0.f, -1.f, 0.f),
        const float& _lifetime = 1.f,
        const float& _lazerLength = 1.f,
        const float& _growTime = 1.f
        );

    BossLazerBody* bossLazerBodyAsset(
        const std::string& _meshName,
        const std::string& _animationName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const Vector3& _startPosition = Vector3(0.f, 0.f, 0.f),
        const Vector3& _direction = Vector3(0.f, -1.f, 0.f),
        const float& _lifetime = 1.f,
        const float& _lazerLength = 1.f,
        const float& _growTime = 1.f
        );
};

#endif // _BOSS_LAZER_BODY_H