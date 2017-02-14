#ifndef _ENTITY_BASE_H
#define _ENTITY_BASE_H

#include "Vector3.h"

class EntityBase
{
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void RenderUI();
	virtual void HandleOutOfBounds(Vector3 min, Vector3 max, double dt);
	virtual void HandleOutOfBounds(Vector3 point, float radius, double dt);
	virtual void CollisionResponse(EntityBase* other, double dt);

	inline void SetPosition(const Vector3& _value){ this->position = _value; };
	inline Vector3 GetPosition(){ return position; };

	inline void SetVelocity(const Vector3& _value){ this->velocity = _value; };
    inline void AddVelocity(const Vector3& _value){ this->velocity += _value; };
	inline Vector3 GetVelocity(){ return this->velocity; };

	inline void SetScale(const Vector3& _value){ this->scale = _value; };
	inline Vector3 GetScale(){ return this->scale; };

    inline void SetIsDone(const bool _bool){ this->b_IsDone = _bool; };
	inline bool GetIsDone(void){ return this->b_IsDone; };

	inline void SetIsDead(const bool _bool){ this->b_IsDead = _bool; };
    inline bool GetIsDead(void){ return this->b_IsDead; };

    inline void SetShouldRender(const bool _bool){ this->b_DoRender = _bool; };
    inline bool GetShouldRender(void){ return this->b_DoRender; };

	// Check if this entity has a collider class parent
	inline void SetHasCollider(const bool _bool){ this->b_HasCollider = _bool; };
	inline bool GetHasCollider(void){ return this->b_HasCollider; };

protected:
	Vector3 position;
	Vector3 velocity;
	Vector3 scale;
	
	bool b_IsDone;
	bool b_IsDead;
	bool b_HasCollider;
    bool b_DoRender;
};

#endif // _ENTITY_BASE_H