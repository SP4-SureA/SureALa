#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "..\SceneBase2D.h"
#include "..\Button.h"

#include "..\SpriteEntity.h"

class PauseState : public SceneBase2D
{
public:
	PauseState();
	virtual ~PauseState();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	GenericEntity* background;
	Button* backBtn;
	Button* menuBtn;
};

#endif