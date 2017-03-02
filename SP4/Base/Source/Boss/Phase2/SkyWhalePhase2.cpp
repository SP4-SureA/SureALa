#include "SkyWhalePhase2.h"

SkyWhalePhase2::SkyWhalePhase2()
{
}

SkyWhalePhase2::~SkyWhalePhase2()
{
}

void SkyWhalePhase2::Init()
{
    SetCurrentState(&angryState);
}

void SkyWhalePhase2::Update(double dt)
{
	StateManager::Update(dt);
}

