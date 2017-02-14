#pragma once

#include "Vector3.h"
#include <vector>
using namespace std;

#include "EntityBase.h"
#include "Transform.h"

class SceneNode : public Transform
{
public:
	SceneNode(void);
	virtual ~SceneNode();

	// Release all memory for this node and its children
	void Destroy(void);

	// Set the ID for this node
	void SetID(const int ID);
	// Get the ID for this node
	int GetID(void) const;

	// Set the entity for this node
	bool SetEntity(EntityBase* theEntity);
	// Get the ID for this node
	EntityBase* GetEntity(void) const;

	// Set a parent to this node
	void SetParent(SceneNode* theParent);
	// Get parent of this node
	SceneNode* GetParent(void) const;

	// Add a child to this node
	SceneNode* AddChild(EntityBase* theEntity = NULL);
	// Delete a child from this node using the pointer to the entity
	bool DeleteChild(EntityBase* theEntity = NULL);
	// Delete a child from this node using its ID
	bool DeleteChild(const int ID);
	// Delete all children from this node using its ID
	bool DeleteAllChildren(void);
	// Detach a child from this node using the pointer to the node
	SceneNode* DetachChild(EntityBase* theEntity = NULL);
	// Detach a child from this node using its ID
	SceneNode* DetachChild(const int ID);
	// Get the entity inside this Scene Graph
	SceneNode* GetEntity(EntityBase* theEntity);
	// Get a child from this node using its ID
	SceneNode* GetEntity(const int ID);
	// Return the number of children in this group
	int GetNumOfChild(void);

	// Update the Scene Graph
	void Update(void);
	// Render the Scene Graph
	void Render(void);

	// PrintSelf for debug purposes
	void PrintSelf(const int numTabs = 0);

protected:
	int			ID;
	EntityBase* theEntity;
	SceneNode* theParent;

	vector<SceneNode*> theChildren;
};
