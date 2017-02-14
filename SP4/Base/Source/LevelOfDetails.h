#ifndef LEVEL_OF_DETAILS_H
#define LEVEL_OF_DETAILS_H

#include "Vector3.h"
#include "Mesh.h"

class LevelOfDetails
{
public:
	LevelOfDetails();
	virtual ~LevelOfDetails();

	enum DETAIL_LEVEL
	{
		NO_DETAILS = 0,
		HIGH_DETAILS,
		MID_DETAILS,
		LOW_DETAILS,

		NUM_DETAIL_LEVEL,
	};

	Mesh* modelMesh_HighDetails;
	Mesh* modelMesh_MidDetails;
	Mesh* modelMesh_LowDetails;
	
	bool InitLOD(
		const std::string& _meshName_High, 
		const std::string& _meshName_Mid,
		const std::string& _meshName_Low
		);
	bool DestroyLOD(void);

	void SetLODStatus(const bool active);
	bool GetLODStatus(void) const;
	bool SetLODMesh(Mesh* mesh, const DETAIL_LEVEL detailLevel);
	Mesh* GetLODMesh(void) const;
	Mesh* GetLODMesh(const DETAIL_LEVEL detailLevel) const;
	int GetDetailLevel(void) const;
	bool SetDetailLevel(const DETAIL_LEVEL detailLevel);

	void UpdateLOD(float distance);
	void SetLevelOfDetailsRange(const float distance_High, const float distance_Mid, const float distance_Low);

protected:
	bool isActive;
	DETAIL_LEVEL detailLevel;
	float LevelOfDetails_Distances[NUM_DETAIL_LEVEL];
};

#endif