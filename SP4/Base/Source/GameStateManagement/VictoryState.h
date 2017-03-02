#ifndef _VICTORYSTATE_H
#define _VICTORYSTATE_H

#include "..\SceneBase2D.h"

class VictoryState : public SceneBase2D
{
public:
    VictoryState();
    virtual ~VictoryState();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	GenericEntity* background;

};

#endif //_VICTORYSTATE_H