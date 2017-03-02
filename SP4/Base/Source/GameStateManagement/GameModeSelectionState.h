#ifndef _GAMEMODE_SELECTION_STATE_H
#define _GAMEMODE_SELECTION_STATE_H

#include "..\SceneBase2D.h"

#include "..\Button.h"

class GameModeSelectionState : public SceneBase2D
{
enum CURR_SELECTION
{
    STORY,
    ENDLESS,
    EXIT,
    NUM_SELECTION
};

public:
	GameModeSelectionState();
	virtual ~GameModeSelectionState();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

    void UpdateMouse();
    void UpdateKeyboard();

protected:
	GenericEntity* background;

    Button* storyBtn;
    Button* endlessBtn;
    Button* exitBtn;
    Button* selectionIcon;

    CURR_SELECTION currentSelection;
};

#endif //_GAMEMODE_SELECTION_STATE_H