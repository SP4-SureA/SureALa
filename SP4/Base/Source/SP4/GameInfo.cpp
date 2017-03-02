#include "GameInfo.h"

#include "..\PlayerInfo\PlayerEntityBase.h"

GameInfo::GameInfo():
health(0),
score(0)
{
}

GameInfo::~GameInfo()
{
}

void GameInfo::Init()
{
	this->health = 5;
	this->score = 0;
}

bool GameInfo::RemovePlayer(PlayerInfo* player)
{
	std::vector<PlayerInfo*>::iterator findIter = std::find(playersList.begin(), playersList.end(), player);

	if (findIter != playersList.end())
	{
		PlayerInfo* placeHolder = *findIter;
		findIter = playersList.erase(findIter);

		delete placeHolder;

		return true;
	}
	// Return false if not found
	return false;
}

PlayerEntityBase* GameInfo::GetNearestPlayer(Vector3 fromPos)
{
	PlayerEntityBase* nearestEntity = NULL;
	float closestDistance = FLT_MAX;

	for (auto q : playersList)
	{
		float distanceSquared = (fromPos - q->GetCharacter()->GetPosition()).LengthSquared();
		if (distanceSquared < closestDistance)
		{
			nearestEntity = q->GetCharacter();
			closestDistance = distanceSquared;
		}
	}

	return nearestEntity;
}

PlayerEntityBase* GameInfo::GetRandomPlayer()
{
	int randValue =rand() % playersList.size();
	
	int counter = 0;
	for (auto q : playersList)
	{
		if (randValue == counter)
		{
			return q->GetCharacter();
		}

		counter++;
	}

	return NULL;
}