#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

#include "..\SceneBase2D.h"
#include "..\Button.h"

#include "..\SpriteEntity.h"

class PauseState : public SceneBase2D
{
public:
	enum BUTTONS
	{
		BTN_RESUME,
		BTN_HELP,
		BTN_MENU,
		BTN_QUIT,

		BTN_END
	};
	PauseState();
	virtual ~PauseState();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	Vector3 mousePos;

	GenericEntity* background;
	GenericEntity* selection;
	Button* buttons[BTN_END];
	int btnIndex;
};

#endif