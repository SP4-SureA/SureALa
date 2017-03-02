#include "SkyWhalePhase1.h"

SkyWhalePhase1::SkyWhalePhase1()
{
}

SkyWhalePhase1::~SkyWhalePhase1()
{
}

void SkyWhalePhase1::Update(double dt)
{
	StateManager::Update(dt);

	if (currentState == &spawnState)
	{
		if (spawnState.GetIfAllGravesAreDead())
		{
            SetShouldChange(true);
		}
	}
}

void SkyWhalePhase1::Init()
{
    SetCurrentState(&spawnState);
}

void SkyWhalePhase1::Exit()
{
    if (currentState)
        currentState->OnExit(attachedEntity);
}