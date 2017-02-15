#include "Grid.h"
#include "stdio.h"
#include "MeshBuilder.h"
#include "RenderHelper.h"
#include "../GenericEntity.h"

/********************************************************************************
Constructor
********************************************************************************/
Grid::Grid(void)
	: index(Vector3(-1, -1, -1))
	, size(Vector3(-1, -1, -1))
	, offset(Vector3(-1, -1, -1))
	, min(Vector3(-1, -1, -1))
	, max(Vector3(-1, -1, -1))
	, theMesh(NULL)
	, ListOfObjects(NULL)
	, detailLevel(LevelOfDetails::NO_DETAILS)
{
}

/********************************************************************************
Destructor
********************************************************************************/
Grid::~Grid(void)
{
	if (theMesh)
	{
		delete theMesh;
		theMesh = NULL;
	}
	Remove();
}

/********************************************************************************
Initialise this grid
********************************************************************************/
void Grid::Init(	const int xIndex, const int zIndex,
					const int xGridSize, const int zGridSize,
					const float xOffset, const float zOffset)
{
	index.Set(xIndex, 0, zIndex);
	size.Set(xGridSize, 0, zGridSize);
	offset.Set(xOffset, 0, zOffset);
	min.Set(index.x * size.x - offset.x, 0.0f, index.z * size.z - offset.z);
	max.Set(index.x * size.x - offset.x + xGridSize, 0.0f, index.z * size.z - offset.z + zGridSize);
}

/********************************************************************************
 Set a particular grid's Mesh
********************************************************************************/
void Grid::SetMesh(const std::string& _meshName)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh != nullptr)
	{
		theMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	}
}

/********************************************************************************
Update the grid
********************************************************************************/
void Grid::Update(vector<EntityBase*>* migrationList)
{
	// Check each object to see if they are no longer in this grid
	std::vector<EntityBase*>::iterator it;
	it = ListOfObjects.begin();
	while (it != ListOfObjects.end())
	{
		Vector3 position = (*it)->GetPosition();

		if (((min.x <= position.x) && (position.x <= max.x)) &&
			((min.z <= position.z) && (position.z <= max.z)))
		{
			// Move on otherwise
			++it;
		}
		else
		{
			migrationList->push_back(*it);

			// Remove from this Grid
			it = ListOfObjects.erase(it);
		}
	}
}

/********************************************************************************
RenderScene
********************************************************************************/
void Grid::Render(void)
{
	if (theMesh)// && ListOfObjects.size() == 0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		RenderHelper::RenderMesh(theMesh);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/********************************************************************************
RenderObjects
********************************************************************************/
void Grid::RenderObjects(const int RESOLUTION)
{
	/*
	glPushAttrib(GL_ENABLE_BIT);
	// Draw the Grid and its list of objects
	for (int i=0; i<(int)ListOfObjects.size(); i++)
	{
	ListOfObjects[i]->Render(RESOLUTION);
	}
	glPopAttrib();
	*/
}

/********************************************************************************
Add a new object to this grid
********************************************************************************/
void Grid::Add(EntityBase* theObject)
{
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		if (ListOfObjects[i] == theObject)
			return;
	}
	ListOfObjects.push_back( theObject );
}

/********************************************************************************
 Remove but not delete object from this grid
********************************************************************************/
void Grid::Remove(void)
{
	for( int i = 0 ; i < ListOfObjects.size(); i++)
	{
		// Do not delete the objects as they are stored in EntityManager and will be deleted there.
		//delete ListOfObjects[i];
		ListOfObjects[i] = NULL;
	}
	ListOfObjects.clear();
}

/********************************************************************************
 Remove but not delete an object from this grid
********************************************************************************/
bool Grid::Remove(EntityBase* theObject)
{
	// Clean up entities that are done
	std::vector<EntityBase*>::iterator it, end;
	it = ListOfObjects.begin();
	end = ListOfObjects.end();
	while (it != end)
	{
		if ((*it) == theObject)
		{
			it = ListOfObjects.erase(it);
			return true;
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
	return false;
}

/********************************************************************************
 Check if an object is in this grid
********************************************************************************/
bool Grid::IsHere(EntityBase* theObject) const
{
	for (int i = 0; i < ListOfObjects.size(); ++i)
	{
		if (ListOfObjects[i] == theObject)
			return true;
	}
	return false;
}

/********************************************************************************
Get list of objects in this grid
********************************************************************************/
vector<EntityBase*> Grid::GetListOfObject(void)
{
	return ListOfObjects;
}

/********************************************************************************
Set the Level of Detail for objects in this Grid
********************************************************************************/
void Grid::SetDetailLevel(const LevelOfDetails::DETAIL_LEVEL detailLevel)
{
	this->detailLevel = detailLevel;

	std::vector<EntityBase*>::iterator it;
	it = ListOfObjects.begin();
	while (it != ListOfObjects.end())
	{
		++it;
	}
}

void Grid::UpdateDetailLevel(Vector3 cameraPosition)
{
	std::vector<EntityBase*>::iterator it;
	it = ListOfObjects.begin();

	while (it != ListOfObjects.end())
	{
		LevelOfDetails* entity = dynamic_cast<LevelOfDetails*>(*it);
		if (entity && entity->GetLODStatus())
		{
			float distance = DistanceSquaredBetween(cameraPosition, (*it)->GetPosition());
			entity->UpdateLOD(distance);
		}
		++it;
	}
}

/********************************************************************************
 PrintSelf
 ********************************************************************************/
void Grid::PrintSelf()
{
	if (ListOfObjects.size() > 0)
	{
	//cout << "Grid::PrintSelf()" << endl;
	//cout << "\tIndex\t:\t" << index << "\t\tOffset\t:\t" << offset << endl;
	//cout << "\tMin\t:\t" << min << "\tMax\t:\t" << max << endl;
	//
	//	cout << "\tList of objects in this grid: (LOD: " << this->detailLevel << ")" << endl;
	//	cout << "\t------------------------------------------------------------------------" << endl;
	//
	//for (int i = 0; i < ListOfObjects.size(); ++i)
	//{
	//	cout << "\t" << i << "\t:\t" << ListOfObjects[i]->GetPosition() << endl;
	//}
	//if (ListOfObjects.size()>0)
	//	cout << "\t------------------------------------------------------------------------" << endl;
	//cout << "********************************************************************************" << endl;

		cout << "********************************************************************************" << endl;
		//cout << index << endl;
		cout << ListOfObjects.size() << endl;
		cout << "********************************************************************************" << endl;
	}
}
