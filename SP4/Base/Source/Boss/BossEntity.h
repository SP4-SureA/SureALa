#ifndef _BOSS_ENTITY_BASE_H
#define _BOSS_ENTITY_BASE_H

#include "Vector3.h"
#include "..\Enemy\EnemyBase.h"
#include "../State/StateManager.h"

#include <vector>
using std::vector;

class BossEntity : public EnemyBase
{
public:
	virtual ~BossEntity();
   
    virtual void Init(){};
    virtual void Update(double dt);
    virtual void Render();

	virtual void AddSpawnedEntity(EntityBase* _entity);
	virtual EntityBase* RemoveSpawnedEntity(EntityBase* _entity);

	virtual bool GetInEntityList(EntityBase* _entity);
    virtual void ClearControlledEntities();
    virtual void HandleControlledEntities();

	inline vector<EntityBase*> GetSpawnedEntityList(){return this->v_SpawnedEntites;};

	inline StateManager* GetCurrentStateManager(){ return this->currentStateManager; };
    void SetCurrentStateManager(StateManager* _StateManager);

	void UpdateVectorSpawnedEntities();

protected:
	StateManager* currentStateManager;
	BossEntity(Mesh* _modelMesh);
	vector<EntityBase*> v_SpawnedEntites;

};

#endif // _BOSS_ENTITY_BASE_H