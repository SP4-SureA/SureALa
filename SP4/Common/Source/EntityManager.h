#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "SingletonTemplate.h"

#include <list>
#include "Vector3.h"

class EntityBase;

class EntityManager : public Singleton<EntityManager>
{
	friend Singleton<EntityManager>;
public:
	EntityManager();
	virtual ~EntityManager();

	virtual void Update(double dt);
	void Render();
	void RenderUI();
	void HandleOutOfBounds(Vector3 min, Vector3 max, double dt);
	void HandleOutOfBounds(Vector3 point, float radius, double dt);

	void AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition = false);
	bool RemoveEntity(EntityBase* _existingEntity);
	bool MarkForDeletion(EntityBase* _existingEntity);
	//void SetSpatialPartition(CSpatialPartition* theSpatialPartition);
	void ClearList();

	std::list<EntityBase*> GetList();

	EntityBase* FindCollision_AABB(EntityBase *thisEntity);
	EntityBase* FindCollision_AABB(Vector3 pos, Vector3 min, Vector3 max, EntityBase* selfCheckHandler = NULL);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity);
	bool CheckAABBCollision(Vector3 pos, Vector3 min, Vector3 max, EntityBase *ThatEntity);

	// Check for intersection between a line segment and a plane
	bool CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end,
		Vector3 minAABB, Vector3 maxAABB,
		Vector3 &Hit);

protected:

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if any Collider is colliding with another Collider
	void CheckForCollision(double dt);

	bool GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	// Check two positions are within a box region
	bool InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);

	std::list<EntityBase*> entityList;
	//CSpatialPartition* spatialPartition;
};

#endif // ENTITY_MANAGER_H