#ifndef COLLIDER_H
#define COLLIDER_H

#include "Vector3.h"

class Collider
{
public:
	Collider();
	~Collider();

	enum COLLIDER_TYPE
	{
		COLLIDER_NONE = 0,

		COLLIDER_BOX,
		COLLIDER_SPHERE,

		NUM_COLLIDER_TYPE,
	};
	
	inline void SetColliderIsActive(bool _bool){ this->isActive = _bool; }
	inline bool GetColliderIsActive(){ return this->isActive; }

	inline void SetColliderType(COLLIDER_TYPE type){ this->colliderType = type; }
	inline COLLIDER_TYPE GetColliderType(){ return this->colliderType; }

	inline void SetColliderOffset(Vector3 offset){ this->colliderOffset = offset; }
	inline Vector3 GetColliderOffset(){ return this->colliderOffset; }

	inline void SetColliderSize(Vector3 size){ this->colliderSize = size; }
	inline Vector3 GetColliderSize(){ return this->colliderSize; }

protected:
	Vector3 colliderOffset;
	Vector3 colliderSize;//x value represents radius for sphere colliders

	COLLIDER_TYPE colliderType;

	bool isTrigger;
	bool isStatic;
	bool isActive;
};

#endif