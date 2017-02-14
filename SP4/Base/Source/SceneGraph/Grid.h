#pragma once

#include "EntityBase.h"
#include "Vector3.h"
#include <vector>
using namespace std;

//Include GLEW
#include <GL/glew.h>
#include "../LevelOfDetails.h"

class Mesh;

class Grid
{
public:
	// Constructor
	Grid(void);
	// Destructor
	~Grid(void);

	// Init
	void Init(	const int xIndex, const int zIndex, 
				const int xGridSize, const int zGridSize,
				const float xOffset = 0, const float zOffset = 0);

	// Set a particular grid's Mesh
	void SetMesh(const std::string& _meshName);

	// Update the grid
	void Update(vector<EntityBase*>* migrationList);
	// Render the grid
	void Render(void);
	// RenderObjects
	void RenderObjects(const int RESOLUTION);

	// Add a new object to this grid
	void Add(EntityBase* theObject);
	// Remove but not delete all objects from this grid
	void Remove(void);
	// Remove but not delete an object from this grid
	bool Remove(EntityBase* theObject);

	// Check if an object is in this grid
	bool IsHere(EntityBase* theObject) const;

	// Get list of objects in this grid
	vector<EntityBase*> GetListOfObject(void);

	// Set the Level of Detail for objects in this grid
	void SetDetailLevel(const LevelOfDetails::DETAIL_LEVEL detailLevel);

	void UpdateDetailLevel(Vector3 cameraPosition);
	// PrintSelf
	void PrintSelf();
	
protected:
	// We use a Vector3 to store the indices of this Grid within the Spatial Partition array.
	Vector3 index;
	// We use a Vector3 to store the size of this Grid within the Spatial Partition array.
	Vector3 size;
	// We use a Vector3 to store the x- and z-offset of this Grid.
	Vector3 offset;
	// We use a Vector3 to store the x- and z-offset of this Grid.
	Vector3 min, max;

	// The mesh to represent the grid
	Mesh* theMesh;

	// List of objects in this grid
	vector<EntityBase*> ListOfObjects;

	// The level of detail for this Grid
	LevelOfDetails::DETAIL_LEVEL detailLevel;
};
