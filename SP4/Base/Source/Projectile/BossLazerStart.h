#ifndef _BOSS_LAZER_START_H
#define _BOSS_LAZER_START_H

#include "Vector3.h"
#include "ProjectileBase.h"
#include "BossLazerBody.h"

class BossLazerStart : public ProjectileBase
{
public:
    BossLazerStart(Mesh* _modelMesh);
    virtual ~BossLazerStart();

    virtual void Update(double dt);
    virtual void Render();

    void UpdateLazer(double dt);

    inline float GetChargeTime(){ return this->chargeTime; };
    inline void SetChargeTime(float _chargeTime){ this->chargeTime = _chargeTime; };

    inline float GetLazerLength(){ return this->lazerLength; };
    inline void SetLazerLength(float _length){ this->lazerLength = _length; };

protected:
    bool firedLazer;
    float chargeTime;
    float lazerLength;
    BossLazerBody* lazerBody;
};

namespace Create
{
    BossLazerStart* bossLazerStartEntity(
        EntityManager* em,
        const std::string& _meshName,
        const std::string& _animationName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const Vector3& _direction = Vector3(1.0f, 1.0f, 1.0f),
        const float& _lifespan = 1.f,
        const float& _ChargeTime = 0.f,
        const float& _LazerLength = 1.f
        );

    BossLazerStart* bossLazerStartAsset(
        const std::string& _meshName,
        const std::string& _animationName,
        const Vector3& _position = Vector3(0, 0, 0),
        const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
        const Vector3& _direction = Vector3(1.0f, 1.0f, 1.0f),
        const float& _lifespan = 1.f,
        const float& _ChargeTime = 0.f,
        const float& _LazerLength = 1.f
        );
};

#endif // _BOSS_LAZER_START_H