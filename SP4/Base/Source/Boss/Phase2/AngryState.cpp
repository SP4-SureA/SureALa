#include "AngryState.h"
#include "EntityManager.h"

#include "EntityBase.h"
#include "../../Boss/Phase1/BossMinion.h"
#include "../SkyWhale.h"
#include "MeshBuilder.h"

AngryState::AngryState() :
currActionCooldownTime(0),
actionCooldown(5)
{
}

AngryState::~AngryState()
{
}

void AngryState::OnEnter(EntityBase* entity)
{
    SkyWhale* boss = dynamic_cast<SkyWhale*>(entity);

    if (!boss)
        return;

    boss->ClearWaypoints();
 
    Vector3 worldTopLeft(boss->GetWorldMin().x + boss->GetScale().x, boss->GetWorldMax().y - boss->GetScale().y * 0.3);
	Vector3 worldTopRight(boss->GetWorldMax().x - boss->GetScale().x, boss->GetWorldMax().y - boss->GetScale().y * 0.3);
    boss->AddWaypoint(worldTopLeft);
    boss->AddWaypoint(worldTopRight);

	boss->SetAnimation("blubby", "idle");

	boss->SetMoveSpeed(boss->GetMoveSpeed() * 2.f);
	boss->SetMaxSpeed(boss->GetMaxSpeed() * 2.f);
}

void AngryState::Update(double dt, EntityBase* entity)
{
    if (!entity)
        return;

    SkyWhale* boss = dynamic_cast<SkyWhale*>(entity);

    if (!boss)
        return;

    UpdateAction(dt, entity);
}

void AngryState::UpdateAction(double dt, EntityBase* entity)
{
    if (currActionCooldownTime > actionCooldown)
    {
        currActionCooldownTime = 0;
        DoAction(dt, entity);
    }

    currActionCooldownTime += dt;
}

void AngryState::DoAction(double dt, EntityBase* entity)
{
    SkyWhale* boss = dynamic_cast<SkyWhale*>(entity);
    if (!boss)
        return;

    int num = rand() % 2;

    if (num == 0)
    {
        SkyWhale* boss = dynamic_cast<SkyWhale*>(entity);
        if (!boss)
            return;

        boss->DischargeLazer();
    }
    else
    {
        boss->DischargeRapidWeapon();
    }

    SpawnBossMinions(dt, entity);
}

void AngryState::OnExit(EntityBase* entity)
{
    BossEntity* boss = dynamic_cast<BossEntity*>(entity);

    if (!boss)
        return;

    boss->ClearControlledEntities();
    boss->ClearWaypoints();

    boss->SetMoveSpeed(boss->GetMoveSpeed() * 0.5f);
    boss->SetMaxSpeed(boss->GetMaxSpeed() * 0.5f);
}

void AngryState::SpawnBossMinions(double dt, EntityBase* entity)
{
    SkyWhale* boss = dynamic_cast<SkyWhale*>(entity);

    if (!boss)
        return;

    for (int i = 0; i < 8; ++i)
    {
        Vector3 randomOffset(rand() % 6 - 3, rand() % 6 - 3, 0);
        boss->AddSpawnedEntity(Create::bossMinionEntity(EntityManager::GetInstance(), "boss_minion", "movement", boss->GetPosition() + randomOffset, Vector3(2, 2, 2), 2, Math::RandFloatMinMax(5, 15), Math::RandFloatMinMax(5, 15), 1));
    }

}
