#ifndef _ENTITY_BASE_H
#define _ENTITY_BASE_H

#include "Vector3.h"
#include "Collider.h"

class EntityBase : public Collider
{
public:
	EntityBase();
	virtual ~EntityBase();

	virtual void UpdateInputs(double dt);
	virtual void UpdateAnimation(double dt);
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

	inline void SetFront(const Vector3& _value){ this->front = _value; };
	inline Vector3 GetFront(){ return this->front; };

	inline void SetScale(const Vector3& _value){ this->scale = _value; };
	inline Vector3 GetScale(){ return this->scale; };

	inline void SetWeight(float weight){ this->weight = weight; }
	inline float GetWeight(){ return this->weight; }
	
    inline void SetIsDone(const bool _bool){ this->b_IsDone = _bool; };
	inline bool GetIsDone(void){ return this->b_IsDone; };

	inline void SetIsDead(const bool _bool){ this->b_IsDead = _bool; };
    inline bool GetIsDead(void){ return this->b_IsDead; };

    inline void SetShouldRender(const bool _bool){ this->b_DoRender = _bool; };
    inline bool GetShouldRender(void){ return this->b_DoRender; };

	inline void SetShouldUpdate(const bool _bool){ this->b_DoUpdate = _bool; };
	inline bool GetShouldUpdate(void){ return this->b_DoUpdate; };

protected:
	Vector3 position;
	Vector3 velocity;
	Vector3 front;
	Vector3 scale;
	float weight;
	
	bool b_IsDone;
	bool b_IsDead;
	bool b_DoUpdate;
    bool b_DoRender;
};

#endif // _ENTITY_BASE_H