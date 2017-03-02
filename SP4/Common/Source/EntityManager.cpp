#include "EntityManager.h"
#include "EntityBase.h"

#include "MeshBuilder.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include <iostream>
using namespace std;

// Update all entities
void EntityManager::Update(double dt)
{
	// Update all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if ((*it)->GetShouldUpdate())
			(*it)->Update(dt);
		(*it)->UpdateAnimation(dt);
	}
	//SceneGraph::GetInstance()->Update();
	//if (spatialPartition)
	//	spatialPartition->Update();

	CheckForCollision(dt);

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if ((*it)->GetIsDone())
		{
			// Delete if done
			//SceneGraph::GetInstance()->DeleteNode((*it));
			delete *it;
			(*it) = NULL;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

void EntityManager::HandleOutOfBounds(Vector3 min, Vector3 max, double dt)
{
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->HandleOutOfBounds(min, max, dt);
	}
}

void EntityManager::HandleOutOfBounds(Vector3 point, float radius, double dt)
{
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->HandleOutOfBounds(point, radius, dt);
	}
}

// Render all entities
void EntityManager::Render()
{
	// Render all entities
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if ((*it)->GetShouldRender())
			(*it)->Render();
	}
	//SceneGraph::GetInstance()->Render();

	//if (spatialPartition)
	//	spatialPartition->Render();
}

// Render the UI entities
void EntityManager::RenderUI()
{
	// Render all entities UI
	std::list<EntityBase*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->RenderUI();
	}
}

// Add an entity to this EntityManager
void EntityManager::AddEntity(EntityBase* _newEntity, bool bAddToSpatialPartition)
{
	entityList.push_back(_newEntity);
	//SceneGraph::GetInstance()->AddNode(_newEntity);
	// Add to the Spatial Partition
	//if (spatialPartition && bAddToSpatialPartition)
	//	spatialPartition->Add(_newEntity);
}

// Remove an entity from this EntityManager
bool EntityManager::RemoveEntity(EntityBase* _existingEntity)
{
	// Find the entity's iterator
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		findIter = entityList.erase(findIter);

		//if (SceneGraph::GetInstance()->DeleteNode(_existingEntity) == false)
		//{
		//	std::cout << "EntityManager::RemoveEntity: Unable to remove" << std::endl;
		//}
		//else
		//{
		//	if (spatialPartition)
		//		spatialPartition->Remove(_existingEntity);
		//}

		return true;
	}
	// Return false if not found
	return false;
}

// Mark an entity for deletion
bool EntityManager::MarkForDeletion(EntityBase* _existingEntity)
{
	std::list<EntityBase*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete entity if found
	if (findIter != entityList.end())
	{
		(*findIter)->SetIsDone(true);
		return true;
	}
	else
		return false;
}

//void EntityManager::SetSpatialPartition(CSpatialPartition* theSpatialPartition)
//{
//	this->spatialPartition = theSpatialPartition;
//}

void EntityManager::ClearList()
{
	std::list<EntityBase*>::iterator it;
	for (it = entityList.begin(); it != entityList.end();)
	{
		//SceneGraph::GetInstance()->DeleteNode((*it));
		it = entityList.erase(it);
	}
}

std::list<EntityBase*> EntityManager::GetList()
{
	return entityList;
}

// Constructor
EntityManager::EntityManager()
//spatialPartition(CSpatialPartition::GetInstance())
{
}

// Destructor
EntityManager::~EntityManager()
{
}

EntityBase* EntityManager::FindCollision_AABB(EntityBase *thisEntity)
{
	//std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;
	//colliderThatEnd = entityList.end();

	//if (thisEntity->GetHasCollider())
	//{
	//	for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
	//	{
	//		if ((*colliderThat) == thisEntity)
	//			continue;

	//		if ((*colliderThat)->GetHasCollider())
	//		{
	//			EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
	//			if (CheckAABBCollision(thisEntity, thatEntity) == true)
	//			{
	//				return thatEntity;
	//			}
	//		}
	//	}
	//}

	return NULL;
}

EntityBase* EntityManager::FindCollision_AABB(Vector3 pos, Vector3 min, Vector3 max, EntityBase* selfCheckHandler)
{
	//std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;
	//colliderThatEnd = entityList.end();

	//GenericEntity temp(NULL);
	//temp.SetPosition(pos);
	//temp.SetAABB(max, min);
	//
	//for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
	//{
	//	if ((*colliderThat) == selfCheckHandler)
	//		continue;

	//	if ((*colliderThat)->HasCollider())
	//	{
	//		if (CheckAABBCollision(&temp, *colliderThat) == true)
	//		{
	//			return *colliderThat;
	//		}
	//	}
	//}
	//
	//return NULL;
	return NULL;
}

// Check for overlap
bool EntityManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		||
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	if (((thisMinAABB >= thatMinAABB) && (thisMinAABB <= thatMaxAABB))
		||
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	if (((thisMinAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB))
		&&
		((thisMaxAABB >= thatMinAABB) && (thisMaxAABB <= thatMaxAABB)))
	{
		return true;
	}

	if (((thatMinAABB >= thisMinAABB) && (thatMinAABB <= thisMaxAABB))
		&&
		((thatMaxAABB >= thisMinAABB) && (thatMaxAABB <= thisMaxAABB)))
	{
		return true;
	}

	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool EntityManager::CheckSphereCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	//CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	//CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	//// Get the minAABB and maxAABB for each entity
	//Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	//Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	//Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	//Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	//if (DistanceSquaredBetween(thisMinAABB, thisMaxAABB) +
	//	DistanceSquaredBetween(thatMinAABB, thatMaxAABB)
	//	>
	//	DistanceSquaredBetween(ThisEntity->GetPosition(), ThatEntity->GetPosition()) * 2.0f)
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}

	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool EntityManager::CheckAABBCollision(EntityBase *ThisEntity, EntityBase *ThatEntity)
{
	//CCollider *thisCollider = dynamic_cast<CCollider*>(ThisEntity);
	//CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	//// Get the minAABB and maxAABB for each entity
	//Vector3 thisMinAABB = ThisEntity->GetPosition() + thisCollider->GetMinAABB();
	//Vector3 thisMaxAABB = ThisEntity->GetPosition() + thisCollider->GetMaxAABB();
	//Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	//Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	//return(
	//	thisMaxAABB.x > thatMinAABB.x &&
	//	thisMinAABB.x < thatMaxAABB.x &&
	//	thisMaxAABB.y > thatMinAABB.y &&
	//	thisMinAABB.y < thatMaxAABB.y &&
	//	thisMaxAABB.z > thatMinAABB.z &&
	//	thisMinAABB.z < thatMaxAABB.z
	//	)
	//	||
	//	(
	//	thatMaxAABB.x > thisMinAABB.x &&
	//	thatMinAABB.x < thisMaxAABB.x &&
	//	thatMaxAABB.y > thisMinAABB.y &&
	//	thatMinAABB.y < thisMaxAABB.y &&
	//	thatMaxAABB.z > thisMinAABB.z &&
	//	thatMinAABB.z < thisMaxAABB.z
	//	);

	////if (CheckOverlap(thisMinAABB, thisMaxAABB, thatMinAABB, thatMaxAABB))
	////	return true;
	////
	////Vector3 altThisMinAABB = Vector3(thisMinAABB.x, thisMinAABB.y, thisMaxAABB.z);
	////Vector3 altThisMaxAABB = Vector3(thisMaxAABB.x, thisMaxAABB.y, thisMinAABB.z);

	////if (CheckOverlap(altThisMinAABB, altThisMaxAABB, thatMinAABB, thatMaxAABB))
	////	return true;

	////return false;

	return false;
}

bool EntityManager::CheckAABBCollision(Vector3 pos, Vector3 min, Vector3 max, EntityBase *ThatEntity)
{
	//CCollider thisCollider;
	//thisCollider.SetAABB(max, min);
	//CCollider *thatCollider = dynamic_cast<CCollider*>(ThatEntity);

	//// Get the minAABB and maxAABB for each entity
	//Vector3 thisMinAABB = pos + thisCollider.GetMinAABB();
	//Vector3 thisMaxAABB = pos + thisCollider.GetMaxAABB();
	//Vector3 thatMinAABB = ThatEntity->GetPosition() + thatCollider->GetMinAABB();
	//Vector3 thatMaxAABB = ThatEntity->GetPosition() + thatCollider->GetMaxAABB();

	//return(
	//	thisMaxAABB.x > thatMinAABB.x &&
	//	thisMinAABB.x < thatMaxAABB.x &&
	//	thisMaxAABB.y > thatMinAABB.y &&
	//	thisMinAABB.y < thatMaxAABB.y &&
	//	thisMaxAABB.z > thatMinAABB.z &&
	//	thisMinAABB.z < thatMaxAABB.z
	//	)
	//	||
	//	(
	//	thatMaxAABB.x > thisMinAABB.x &&
	//	thatMinAABB.x < thisMaxAABB.x &&
	//	thatMaxAABB.y > thisMinAABB.y &&
	//	thatMinAABB.y < thisMaxAABB.y &&
	//	thatMaxAABB.z > thisMinAABB.z &&
	//	thatMinAABB.z < thisMaxAABB.z
	//	);

	return false;
}

// Check if any Collider is colliding with another Collider
void EntityManager::CheckForCollision(double dt)
{
	//std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	//std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	//colliderThisEnd = entityList.end();
	//for (colliderThis = entityList.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	//{
	//	if ((*colliderThis)->GetIsLaser())
	//	{
	//		// Dynamic cast it to a CLaser class
	//		CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

	//		// Check for collision with another collider class
	//		colliderThatEnd = entityList.end();
	//		int counter = 0;
	//		for (colliderThat = entityList.begin(); colliderThat != colliderThatEnd; ++colliderThat)
	//		{
	//			if (colliderThat == colliderThis)
	//				continue;

	//			if ((*colliderThat)->HasCollider())
	//			{
	//				Vector3 hitPosition = Vector3(0, 0, 0);
	//				EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);

	//				// Get the minAABB and maxAABB for (*colliderThat)
	//				CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
	//				Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
	//				Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

	//				if (CheckLineSegmentPlane(thisEntity->GetPosition(),
	//					thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
	//					thatMinAABB, thatMaxAABB,
	//					hitPosition) == true)
	//				{
	//					thisEntity->HandleCollision(thatEntity, dt);
	//					thatEntity->HandleCollision(thisEntity, dt);
	//				}
	//			}
	//		}
	//	}
	//	else if ((*colliderThis)->HasCollider())
	//	{
	//		EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

	//		colliderThatEnd = entityList.end();

	//		for (colliderThat = colliderThis; colliderThat != colliderThatEnd; ++colliderThat)
	//		{
	//			if (colliderThat == colliderThis)
	//				continue;

	//			if ((*colliderThat)->HasCollider())
	//			{
	//				EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
	//				if (CheckAABBCollision(thisEntity, thatEntity) == true)
	//				{
	//					thisEntity->HandleCollision(thatEntity, dt);
	//					thatEntity->HandleCollision(thisEntity, dt);
	//				}
	//			}
	//		}
	//	}
	//}
}

bool EntityManager::GetIntersection(const float fDst1, const float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit)
{
	if ((fDst1 * fDst2) >= 0.0f)
		return false;
	if (fDst1 == fDst2)
		return false;
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return true;
}

bool EntityManager::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return true;
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return true;
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return true;
	return false;
}

bool EntityManager::CheckLineSegmentPlane(Vector3 line_start, Vector3 line_end,
	Vector3 minAABB, Vector3 maxAABB,
	Vector3 &Hit)
{
	if ((GetIntersection(line_start.x - minAABB.x, line_end.x - minAABB.x, line_start, line_end, Hit) &&
		InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - minAABB.y, line_end.y - minAABB.y, line_start, line_end, Hit) &&
		InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - minAABB.z, line_end.z - minAABB.z, line_start, line_end, Hit) &&
		InBox(Hit, minAABB, maxAABB, 3))
		|| (GetIntersection(line_start.x - maxAABB.x, line_end.x - maxAABB.x, line_start, line_end, Hit) &&
		InBox(Hit, minAABB, maxAABB, 1))
		|| (GetIntersection(line_start.y - maxAABB.y, line_end.y - maxAABB.y, line_start, line_end, Hit) &&
		InBox(Hit, minAABB, maxAABB, 2))
		|| (GetIntersection(line_start.z - maxAABB.z, line_end.z - maxAABB.z, line_start, line_end, Hit) &&
		InBox(Hit, minAABB, maxAABB, 3)))
		return true;

	return false;
}