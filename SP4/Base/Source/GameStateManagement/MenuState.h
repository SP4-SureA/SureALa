#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "..\SceneBase2D.h"

#include "..\Button.h"

class MenuState : public SceneBase2D
{
enum CURR_SELECTION
{
    PLAY = 0,
    HELP,
    OPTION,
    EXIT,
    NUM_SELECTION
};

public:
	MenuState();
	virtual ~MenuState();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

    void UpdateMouse();
    void UpdateKeyboard();

protected:
	GenericEntity* background;

	Button* playBtn;
    Button* optionBtn;
    Button* helpBtn;
    Button* exitBtn;
    Button* selectionIcon;

    CURR_SELECTION currentSelection;
};

#endif //MENU_STATE_H