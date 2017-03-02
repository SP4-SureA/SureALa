#ifndef _ENEMY_ENTITY_BASE_H
#define _ENEMY_ENTITY_BASE_H

#include "Vector3.h"
#include "../GenericEntity.h"
#include "Animation.h"
#include "AnimationManager.h"

#include <vector>
using std::vector;

class WeaponBase;
class StateManager;

class EnemyBase : public GenericEntity
{
public:
    virtual ~EnemyBase();

    virtual void Update(double dt);
	virtual void UpdateAnimation(double dt){}

    virtual void Render();

	//Action functions
	virtual void MoveTo(Vector3 point, double dt);
	virtual void MoveTowards(Vector3 dir, double dt);
	virtual void Patrol(double dt);
	virtual void Attack(){}

    virtual void HandleState(double dt){}

	inline void SetMoveSpeed(const float& _moveSpeed){ this->moveSpeed = _moveSpeed; };
    inline float GetMoveSpeed(void){ return this->moveSpeed; };

	inline void SetMaxSpeed(const float& _maxSpeed){ this->maxSpeed = _maxSpeed; };
	inline float GetMaxSpeed(void){ return this->maxSpeed; };

    virtual bool TakeDamage(float amount, EntityBase* other = NULL);

	inline void SetHealth(const float& _health){ this->health = _health; };
	inline float GetHealth(void){ return this->health; };

	inline void SetMaxHealth(const float& _health){ this->maxHealth = _health; };
	inline float GetMaxHealth(void){ return this->maxHealth; };

	inline void SetWeapon(WeaponBase* _weapon){ this->weapon = _weapon; };
	inline WeaponBase* GetWeapon(void){ return this->weapon; };

	inline void AddWaypoint(Vector3 point){ this->waypoints.push_back(point); }
	inline void ClearWaypoints(){ this->waypoints.clear(); }
	inline Vector3 GetWaypoint(int index){ if (index < waypoints.size()){ return waypoints[index]; } else { return Vector3(); } }
	Vector3 GetNearestWaypoint();

	inline void SetAnimation(std::string _meshName, std::string _animationName){ this->animationPlayer.m_anim = AnimationManager::GetInstance(_meshName)->GetAnimation(_animationName); animationPlayer.m_pause = false; }
	inline AnimationPlayer* GetAnimationPlayer(){ return &this->animationPlayer; };
protected:
	EnemyBase(Mesh* _modelMesh);

	bool ReachedWaypont(float offset = 0.0f);
	
	float health;
	float maxHealth;
    float moveSpeed;
	float maxSpeed;
	WeaponBase* weapon;

	StateManager* stateManager;

	vector<Vector3> waypoints;
	int waypointIndex;

	AnimationPlayer animationPlayer;
};

#endif // _ENEMY_ENTITY_BASE_H