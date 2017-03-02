#ifndef CAMERA_BASE_H
#define CAMERA_BASE_H

#include "Mtx44.h"
#include "Vector3.h"

class CameraBase
{
public:
	CameraBase();
	virtual ~CameraBase();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up){}
	virtual void Update(double dt){}
	virtual void Reset(){}

	virtual Mtx44 GetViewMatrix()
    {
        Mtx44 result;
        result.SetToLookAt(position.x, position.y, position.z,
            target.x, target.y, target.z,
            up.x, up.y, up.z);
        return result;
    }

	inline Vector3 GetPosition(){ return this->position; }
	inline void SetPosition(Vector3 pos){ this->position = pos; }

	inline Vector3 GetTarget(){ return this->target; }
	inline void SetTarget(Vector3 tar){ this->target = tar; }

	inline Vector3 GetUp(){ return this->up; }
	inline void SetUp(Vector3 _up){ this->up = _up; }

protected:
	Vector3 position;
	Vector3 target;
	Vector3 up;
};

#endif // CAMERA_BASE_H