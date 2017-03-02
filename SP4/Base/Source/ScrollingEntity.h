#ifndef SCROLLING_ENTITY_H
#define SCROLLING_ENTITY_H

#include "EntityBase.h"
#include <string>

#include <vector>
using std::vector;

class Mesh;
class EntityManager;

class ScrollingEntity : public EntityBase
{
public:
	ScrollingEntity(Mesh* _modelMesh);
	virtual ~ScrollingEntity();

	virtual void Update(double dt);
	virtual void RenderUI();

	inline void SetScrollDirection(Vector3 dir){ this->scrollDir = dir; }
	inline Vector3 GetScrollDirection(){ return this->scrollDir; }

	inline void SetScrollSpeed(float speed){ this->scrollSpeed = speed; }
	inline Vector3 GetScrollSpeed(){ return this->scrollSpeed; }

	inline void SetMinBoundary(Vector3 bound){ this->minBoundary = bound; }
	inline Vector3 GetMinBoundaray(){ return this->minBoundary; }
	inline void SetMaxBoundary(Vector3 bound){ this->maxBoundary = bound; }
	inline Vector3 GetMaxBoundaray(){ return this->maxBoundary; }

protected:
	Mesh* modelMesh;

	Vector3 scrollDir;
	float scrollSpeed;

	Vector3 minBoundary;
	Vector3 maxBoundary;
};

namespace Create
{
	ScrollingEntity* scrollingEntity(EntityManager* em,
		const std::string& _meshName,
		const Vector3& _position,
		const Vector3& _scrollDirection,
		const float& _scrollSpeed,
		const Vector3& _scrollBoundaryMin,
		const Vector3& _scrollBoundaryMax,
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f));
};

#endif // GENERIC_ENTITY_H