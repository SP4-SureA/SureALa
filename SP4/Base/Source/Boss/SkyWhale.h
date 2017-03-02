#ifndef _SKY_WHALE_H
#define _SKY_WHALE_H

#include "Vector3.h"
#include "BossEntity.h"
#include "../Boss/Phase1/SkyWhalePhase1.h"
#include "../Boss/Phase2/SkyWhalePhase2.h"

class SkyWhale : public BossEntity
{
public:
    SkyWhale(Mesh* _modelMesh);
	virtual ~SkyWhale();

    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();

    void UpdateWeapons(double dt);
    void DischargeLazer();
    void DischargeRapidWeapon();
	
	inline Vector3 GetWorldMin(){ return this->worldMin; };
	inline void SetWorldMin(Vector3 _worldMin){ this->worldMin = _worldMin; };

	inline Vector3 GetWorldMax(){ return this->worldMax; };
	inline void SetWorldMax(Vector3 _worldMax){ this->worldMax = _worldMax; };

    inline float GetMinionSpawnTime(){ return this->minionSpawnTime; };
    inline void SetMinionSpawnTime(float _minionSpawnTime){ this->minionSpawnTime = _minionSpawnTime; };

    virtual void HandleOutOfBounds(Vector3 min, Vector3 max, double dt);
    virtual bool TakeDamage(float amount, EntityBase* other = NULL);

    inline void SetSecondaryWeapon(WeaponBase* _weapon){ this->secondWeapon = _weapon; };
    inline WeaponBase* GetSecondaryWeapon(void){ return this->secondWeapon; };

	virtual void CollisionResponse(EntityBase* other, double dt);

    void SetSwitchPhase();

    bool isAngry = false;

protected:
    float minionSpawnTime;

	Vector3 worldMin;
	Vector3 worldMax;

	SkyWhalePhase1 phase1StateMachine;
    SkyWhalePhase2 phase2StateMachine;

    WeaponBase* secondWeapon;

};

namespace Create
{
	SkyWhale* skywhaleEntity(
		EntityManager* em,
		const std::string& _meshName,
		const std::string& _animationName,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const Vector3& _worldMin = Vector3(0, 0, 0),
		const Vector3& _worldMax = Vector3(0, 0, 0),
        const float& _speed = 0.f,
        const float& _maxSpeed = 0.f,
        const float& _health = 0.f,
        const float& _minionSpawnRate = 0.f
		);

	SkyWhale* skywhaleAsset(
		const std::string& _meshName,
		const std::string& _animationName,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const Vector3& _worldMin = Vector3(0, 0, 0),
		const Vector3& _worldMax = Vector3(0, 0, 0),
        const float& _speed = 0.f,
        const float& _maxSpeed = 0.f,
        const float& _health = 0.f,
        const float& _minionSpawnRate = 0.f
		);
};

#endif // _SKY_WHALE_H