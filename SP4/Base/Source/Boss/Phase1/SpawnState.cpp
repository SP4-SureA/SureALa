#include "SpawnState.h"
#include "EntityManager.h"
#include "GraveStoneEntity.h"

#include "EntityBase.h"
#include "../../Boss/Phase1/BossMinion.h"
#include "../SkyWhale.h"
#include "../../PlayerInfo/PlayerInfo.h"


SpawnState::SpawnState() :
countdownTime(0),
maxNumberOfGraves(1)
{
}

SpawnState::~SpawnState()
{
}

void SpawnState::OnEnter(EntityBase* entity)
{
    SkyWhale* boss = dynamic_cast<SkyWhale*>(entity);

    if (!boss)
        return;

    for (int i = 0; i < 10; ++i)
    {
        Vector3 randomWorldPosition(Math::RandFloatMinMax(boss->GetWorldMin().x, boss->GetWorldMax().x), Math::RandFloatMinMax(boss->GetWorldMin().y, boss->GetWorldMax().y), 0);
        boss->AddWaypoint(randomWorldPosition);
    }

    { // Init Graves
        float graveSize = 5.0f;

        Vector3 randomWorldPosition = Vector3(Math::RandFloatMinMax(boss->GetWorldMin().x, boss->GetWorldMax().x), Math::RandFloatMinMax(boss->GetWorldMin().y, boss->GetWorldMax().y), 0);
        p_graveEntityVector.push_back(Create::gravestoneEntity(EntityManager::GetInstance(), "bossroomobjects", "grave_melee", GraveStoneEntity::GRAVE_MELEE, randomWorldPosition, Vector3(graveSize, graveSize, 1), 0.5f));
        
        randomWorldPosition = Vector3(Math::RandFloatMinMax(boss->GetWorldMin().x, boss->GetWorldMax().x), Math::RandFloatMinMax(boss->GetWorldMin().y, boss->GetWorldMax().y), 0);
        p_graveEntityVector.push_back(Create::gravestoneEntity(EntityManager::GetInstance(), "bossroomobjects", "grave_range", GraveStoneEntity::GRAVE_RANGE, randomWorldPosition, Vector3(graveSize, graveSize, 1), 0.5f));
        
        randomWorldPosition = Vector3(Math::RandFloatMinMax(boss->GetWorldMin().x, boss->GetWorldMax().x), Math::RandFloatMinMax(boss->GetWorldMin().y, boss->GetWorldMax().y), 0);
        p_graveEntityVector.push_back(Create::gravestoneEntity(EntityManager::GetInstance(), "bossroomobjects", "grave_mix", GraveStoneEntity::GRAVE_MIX, randomWorldPosition, Vector3(graveSize, graveSize, 1), 0.5f));
        
        randomWorldPosition = Vector3(Math::RandFloatMinMax(boss->GetWorldMin().x, boss->GetWorldMax().x), Math::RandFloatMinMax(boss->GetWorldMin().y, boss->GetWorldMax().y), 0);
        p_graveEntityVector.push_back(Create::gravestoneEntity(EntityManager::GetInstance(), "bossroomobjects", "grave_melee", GraveStoneEntity::GRAVE_MELEE, randomWorldPosition, Vector3(graveSize, graveSize, 1), 0.5f));
        
        randomWorldPosition = Vector3(Math::RandFloatMinMax(boss->GetWorldMin().x, boss->GetWorldMax().x), Math::RandFloatMinMax(boss->GetWorldMin().y, boss->GetWorldMax().y), 0);
        p_graveEntityVector.push_back(Create::gravestoneEntity(EntityManager::GetInstance(), "bossroomobjects", "grave_range", GraveStoneEntity::GRAVE_RANGE, randomWorldPosition, Vector3(graveSize, graveSize, 1), 0.5f));

        randomWorldPosition = Vector3(Math::RandFloatMinMax(boss->GetWorldMin().x, boss->GetWorldMax().x), Math::RandFloatMinMax(boss->GetWorldMin().y, boss->GetWorldMax().y), 0);
        p_graveEntityVector.push_back(Create::gravestoneEntity(EntityManager::GetInstance(), "bossroomobjects", "grave_mix", GraveStoneEntity::GRAVE_MIX, randomWorldPosition, Vector3(graveSize, graveSize, 1), 0.5f));
    }

    maxNumberOfGraves = p_graveEntityVector.size();
}

void SpawnState::Update(double dt, EntityBase* entity)
{
	if (!entity)
		return;

    BossEntity* boss = dynamic_cast<BossEntity*>(entity);

    if (!boss)
        return;

    SpawnBossMinions(dt, boss);
	UpdateGraveVector(dt);
}

void SpawnState::OnExit(EntityBase* entity)
{
    BossEntity* boss = dynamic_cast<BossEntity*>(entity);

    if (!boss)
        return;

    boss->ClearControlledEntities();
    boss->ClearWaypoints();
}

void SpawnState::SpawnBossMinions(double dt, BossEntity* entity)
{
    SkyWhale* boss = dynamic_cast<SkyWhale*>(entity);

    if (!boss)
        return;

    if (countdownTime > boss->GetMinionSpawnTime())
	{
		for (int i = 0; i < 5; ++i)
		{
			Vector3 randomOffset(rand() % 6 - 3, rand() % 6 - 3, 0);
			boss->AddSpawnedEntity(Create::bossMinionEntity(EntityManager::GetInstance(), "boss_minion", "movement", boss->GetPosition() + randomOffset, Vector3(2, 2, 2), 2, Math::RandFloatMinMax(5, 15), Math::RandFloatMinMax(5, 15), 1));
		}
        countdownTime = 0;
	}

    countdownTime += dt;
}

void SpawnState::UpdateGraveVector(double dt)
{
	vector<EntityBase*>::iterator it = p_graveEntityVector.begin();
	vector<EntityBase*>::iterator end = p_graveEntityVector.end();

	for (it = p_graveEntityVector.begin(); it != p_graveEntityVector.end();)
	{
		EntityBase* temp = (*it);

		if (temp->GetIsDead() || temp->GetIsDone())
		{
            it = p_graveEntityVector.erase(it);
		}
		else
		{
			++it;
		}
	}
}