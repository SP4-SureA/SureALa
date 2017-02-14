#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "..\SceneBase2D.h"

#include "..\Button.h"

class MenuState : public SceneBase2D
{
public:
	MenuState();
	virtual ~MenuState();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	GenericEntity* background;
	Button* playBtn;
};

#endif