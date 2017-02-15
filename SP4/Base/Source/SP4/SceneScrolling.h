#ifndef SCENE_SCROLLING_H
#define SCENE_SCROLLING_H

#include "..\SceneBase2D.h"
#include "..\GenericEntity.h"

class SceneScrolling : public SceneBase2D
{
public:
	SceneScrolling();
	virtual ~SceneScrolling();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	GenericEntity* groundEntity;

};

#endif