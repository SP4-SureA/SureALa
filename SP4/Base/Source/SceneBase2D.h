#ifndef SCENE_BASE_2D_H
#define SCENE_BASE_2D_H

#include "Scene.h"
#include "Mtx44.h"
#include "PlayerInfo/PlayerInfo.h"
#include "GroundEntity.h"
#include "FPSCamera.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "GenericEntity.h"

class ShaderProgram;
class SceneManager;
class TextEntity;
class Light;
class SceneBase2D : public Scene
{
public:
	SceneBase2D();
	virtual ~SceneBase2D();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	float canvasWidth, canvasHeight;//for UI placements
	float worldWidth, worldHeight;//for game entites
	float orthoWidth, orthoHeight;//for camera view size

	ShaderProgram* currProg;
	CameraBase* camera;
	Light* lights[2];
};

#endif