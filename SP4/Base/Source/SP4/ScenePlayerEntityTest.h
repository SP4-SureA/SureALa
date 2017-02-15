#ifndef _SCENE_PLAYER_ENTITY_TEST_H
#define _SCENE_PLAYER_ENTITY_TEST_H

#include "..\SceneBase2D.h"
#include "..\GenericEntity.h"
#include "../PlayerInfo/PlayerEntityBase.h"

class ScenePlayerEntityTest : public SceneBase2D
{
public:
	ScenePlayerEntityTest();
	virtual ~ScenePlayerEntityTest();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	GenericEntity* testy;
	GenericEntity* testy2;
    PlayerEntityBase* playerEntity;
};

#endif // _SCENE_PLAYER_ENTITY_TEST_H