#ifndef SCENE_TEST_H
#define SCENE_TEST_H

#include "..\SceneBase2D.h"
#include "..\GenericEntity.h"
class SceneTest : public SceneBase2D
{
public:
	SceneTest();
	virtual ~SceneTest();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	GenericEntity* testy;
};

#endif