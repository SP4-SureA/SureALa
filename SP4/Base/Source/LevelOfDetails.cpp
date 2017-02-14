#include "LevelOfDetails.h"

#include "MeshBuilder.h"

/********************************************************************************
Constructor
********************************************************************************/
LevelOfDetails::LevelOfDetails()
	: modelMesh_HighDetails(NULL)
	, modelMesh_MidDetails(NULL)
	, modelMesh_LowDetails(NULL)
	, isActive(false)
	, detailLevel(HIGH_DETAILS)
{
}

/********************************************************************************
Destructor
********************************************************************************/
LevelOfDetails::~LevelOfDetails()
{
	modelMesh_HighDetails = NULL;
	modelMesh_MidDetails = NULL;
	modelMesh_LowDetails = NULL;
}

/********************************************************************************
Initialise the LOD system
********************************************************************************/
bool LevelOfDetails::InitLOD(const std::string& _meshName_High,
	const std::string& _meshName_Mid,
	const std::string& _meshName_Low)
{
	// Retrieve the Meshes from MeshBuilder's library and assign to the 3 Mesh pointers
	modelMesh_HighDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_High);
	modelMesh_MidDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_Mid);
	modelMesh_LowDetails = MeshBuilder::GetInstance()->GetMesh(_meshName_Low);

	// If any of the mesh is not loaded, then return false and avoid setting the LOD to active
	if ((modelMesh_HighDetails == nullptr) ||
		(modelMesh_MidDetails == nullptr) ||
		(modelMesh_LowDetails == nullptr))
		return false;
	// Set the LOD to active
	SetLODStatus(true);
	return true;
}

/********************************************************************************
Destroy the LOD system
********************************************************************************/
bool LevelOfDetails::DestroyLOD(void)
{
	if (modelMesh_HighDetails)
	{
		delete modelMesh_HighDetails;
		modelMesh_HighDetails = NULL;
	}
	if (modelMesh_MidDetails)
	{
		delete modelMesh_MidDetails;
		modelMesh_MidDetails = NULL;
	}
	if (modelMesh_LowDetails)
	{
		delete modelMesh_LowDetails;
		modelMesh_LowDetails = NULL;
	}
	return false;
}

/********************************************************************************
Set the status of the LOD
********************************************************************************/
void LevelOfDetails::SetLODStatus(const bool active)
{
	this->isActive = active;
}
/********************************************************************************
Get the LOD Status
********************************************************************************/
bool LevelOfDetails::GetLODStatus(void) const
{
	return isActive;
}

/********************************************************************************
Set the Mesh for a specified detail level
********************************************************************************/
bool LevelOfDetails::SetLODMesh(Mesh* theMesh, const DETAIL_LEVEL theDetailLevel)
{
	if (theDetailLevel == HIGH_DETAILS)
		modelMesh_HighDetails = theMesh;

	else if (theDetailLevel == MID_DETAILS)
		modelMesh_MidDetails = theMesh;

	else if (theDetailLevel == LOW_DETAILS)
		modelMesh_LowDetails = theMesh;

	else
		return false;

	return true;
}

/********************************************************************************
Get the Mesh based on the current detail level
********************************************************************************/
Mesh* LevelOfDetails::GetLODMesh(void) const
{
	if (detailLevel == HIGH_DETAILS)
		return modelMesh_HighDetails;

	if (detailLevel == MID_DETAILS)
		return modelMesh_MidDetails;

	if (detailLevel == LOW_DETAILS)
		return modelMesh_LowDetails;

	return NULL;
}

/********************************************************************************
Get the Mesh based on the specified detail level
********************************************************************************/
Mesh* LevelOfDetails::GetLODMesh(const DETAIL_LEVEL theDetailLevel) const
{
	if (theDetailLevel == HIGH_DETAILS)
		return modelMesh_HighDetails;

	if (theDetailLevel == MID_DETAILS)
		return modelMesh_MidDetails;

	if (theDetailLevel == LOW_DETAILS)
		return modelMesh_LowDetails;

	return NULL;
}

/********************************************************************************
Get the current detail level
********************************************************************************/
int LevelOfDetails::GetDetailLevel(void) const
{
	return detailLevel;
}

/********************************************************************************
Set the current detail level
********************************************************************************/
bool LevelOfDetails::SetDetailLevel(const DETAIL_LEVEL detailLevel)
{
	if ((detailLevel >= NO_DETAILS) && (detailLevel < NUM_DETAIL_LEVEL))
	{
		this->detailLevel = detailLevel;
		return true;
	}
	return false;
}


void LevelOfDetails::UpdateLOD(float distance)
{
	if (distance < this->LevelOfDetails_Distances[HIGH_DETAILS] * this->LevelOfDetails_Distances[HIGH_DETAILS])
	{
		detailLevel = HIGH_DETAILS;
		return;
	}
	else if (distance < this->LevelOfDetails_Distances[MID_DETAILS] * this->LevelOfDetails_Distances[MID_DETAILS])
	{
		detailLevel = MID_DETAILS;
		return;
	}
	else if (distance < this->LevelOfDetails_Distances[LOW_DETAILS] * this->LevelOfDetails_Distances[LOW_DETAILS])
	{
		detailLevel = LOW_DETAILS;
		return;
	}
	else
	{
		detailLevel = NO_DETAILS;
		return;
	}
}

void LevelOfDetails::SetLevelOfDetailsRange(const float distance_High, const float distance_Mid, const float distance_Low)
{
	this->LevelOfDetails_Distances[HIGH_DETAILS] = distance_High;
	this->LevelOfDetails_Distances[MID_DETAILS] = distance_Mid;
	this->LevelOfDetails_Distances[LOW_DETAILS] = distance_Low;
}



