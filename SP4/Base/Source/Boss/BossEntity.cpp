#include "BossEntity.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

BossEntity::BossEntity(Mesh* _modelMesh) :
EnemyBase(_modelMesh)
{
}

BossEntity::~BossEntity()
{
}

void BossEntity::Update(double dt)
{
	EnemyBase::Update(dt);
    HandleControlledEntities();
	if (currentStateManager)
		currentStateManager->Update(dt);
}

void BossEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

void BossEntity::AddSpawnedEntity(EntityBase* _entity)
{
	if (!GetInEntityList(_entity))
	{
		v_SpawnedEntites.push_back(_entity);
	}
}

EntityBase* BossEntity::RemoveSpawnedEntity(EntityBase* _entity)
{
	vector<EntityBase*>::iterator it = v_SpawnedEntites.begin();
	vector<EntityBase*>::iterator end = v_SpawnedEntites.end();

	for (it; it != end;)
	{
		EntityBase* temp = (*it);

		if (temp == _entity)
		{
			it = v_SpawnedEntites.erase(it);
			return temp;
		}
		else
		{
			++it;
		}
	}
}

void BossEntity::ClearControlledEntities()
{
    vector<EntityBase*>::iterator it = v_SpawnedEntites.begin();
    vector<EntityBase*>::iterator end = v_SpawnedEntites.end();

    for (; it != end; ++it)
    {
        EntityBase* temp = (*it);
        temp->SetIsDead(true);
    }

	v_SpawnedEntites.clear();
}

void BossEntity::UpdateVectorSpawnedEntities()
{
	for (vector<EntityBase*>::iterator it = v_SpawnedEntites.begin(); it != v_SpawnedEntites.end();)
	{
		if (!(*it))
		{
			it = v_SpawnedEntites.erase(it);
			continue;
		}

		if ((*it)->GetIsDead() || (*it)->GetIsDone())
		{
			it = v_SpawnedEntites.erase(it);
		}
		else
		{
			++it;
		}
	}
}

bool BossEntity::GetInEntityList(EntityBase* _entity)
{
	for (auto it : v_SpawnedEntites)
	{
		if (_entity == it)
			return true;
	}

	return false;
}

void BossEntity::SetCurrentStateManager(StateManager* _StateManager)
{
    if (currentStateManager)
        currentStateManager->Exit();


    this->currentStateManager = _StateManager; 

    if (currentStateManager)
        currentStateManager->Init();
}

void BossEntity::HandleControlledEntities()
{
    vector<EntityBase*>::iterator it = v_SpawnedEntites.begin();
    vector<EntityBase*>::iterator end = v_SpawnedEntites.end();

    for (it = v_SpawnedEntites.begin(); it != v_SpawnedEntites.end();)
    {
        EntityBase* temp = (*it);

        if (temp->GetIsDead() || temp->GetIsDone())
        {
            it = v_SpawnedEntites.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
