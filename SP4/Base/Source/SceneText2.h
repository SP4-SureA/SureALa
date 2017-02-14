#ifndef SCENE_TEXT2_H
#define SCENE_TEXT2_H

#include "SceneBase2D.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText2 : public SceneBase2D
{
public:
	SceneText2();
	~SceneText2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	//CPlayerInfo* playerInfo;

	GenericEntity* theCube;

};

#endif