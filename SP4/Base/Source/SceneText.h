#ifndef SCENE_TEXT_H
#define SCENE_TEXT_H

#include "SceneBase3D.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneText : public SceneBase3D
{
public:
	SceneText();
	~SceneText();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:

	//CPlayerInfo* playerInfo;
	GroundEntity* groundEntity;
};

#endif