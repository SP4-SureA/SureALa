#ifndef SCROLLING_CAMERA_H
#define SCROLLING_CAMERA_H

#include "CameraBase.h"

#include <vector>
using std::vector;

class EntityBase;

class ScrollingCamera : public CameraBase
{
public:
	ScrollingCamera();
	virtual ~ScrollingCamera();

	virtual Mtx44 GetViewMatrix();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	inline Vector3 GetOrthoSize(){ return this->orthoSize; }

	inline void SetMinOrthoSize(Vector3 min){ this->minOrthoSize = min; }
	inline Vector3 GetMinOrthoSize(){ return this->minOrthoSize; }

	inline void SetMaxOrthoSize(Vector3 max){ this->maxOrthoSize = max; }
	inline Vector3 GetMaxOrthoSize(){ return this->maxOrthoSize; }

	inline void SetWorldMin(Vector3 min){ this->worldMin = min; }
	inline void SetWorldMax(Vector3 max){ this->worldMax = max; }

	void AddToWatch(EntityBase* entity);
	bool RemoveFromWatch(EntityBase* entity);

private:
	std::vector<EntityBase*> watchList;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Vector3 orthoSize;
	Vector3 minOrthoSize;
	Vector3 maxOrthoSize;
	Vector3 worldMin;
	Vector3 worldMax;
};

#endif