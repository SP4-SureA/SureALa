#include "SceneBase2D.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"

#include <iostream>
using namespace std;

SceneBase2D::SceneBase2D():
camera(NULL)
{
}

SceneBase2D::~SceneBase2D()
{
	if (camera)
		delete camera;
}

void SceneBase2D::Init()
{
	Math::InitRNG();

	//lights[0] = new Light();
	//GraphicsManager::GetInstance()->AddLight("lights[0]", lights[0]);
	//lights[0]->type = Light::LIGHT_DIRECTIONAL;
	//lights[0]->position.Set(0, 0.1f, 1.0f);
	//lights[0]->color.Set(1, 1, 1);
	//lights[0]->power = 1;
	//lights[0]->kC = 1.f;
	//lights[0]->kL = 0.01f;
	//lights[0]->kQ = 0.001f;
	//lights[0]->cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[0]->cosInner = cos(Math::DegreeToRadian(30));
	//lights[0]->exponent = 3.f;
	//lights[0]->spotDirection.Set(0.f, 1.f, 0.f);
	//lights[0]->name = "lights[0]";

	//lights[1] = new Light();
	//GraphicsManager::GetInstance()->AddLight("lights[1]", lights[1]);
	//lights[1]->type = Light::LIGHT_DIRECTIONAL;
	//lights[1]->position.Set(1, 1, 0);
	//lights[1]->color.Set(1, 1, 0.5f);
	//lights[1]->power = 0.4f;
	//lights[1]->name = "lights[1]";

	canvasWidth = Application::GetInstance().GetWindowWidth();
	canvasHeight = Application::GetInstance().GetWindowHeight();
	orthoHeight = 100;
	orthoWidth = orthoHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());
	worldHeight = 100;
	worldWidth = worldHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());

	// Create and attach the camera to the scene
	camera = new FPSCamera();
	camera->Init(Vector3(worldWidth * 0.5f, worldHeight * 0.5f, 1), Vector3(worldWidth * 0.5f, worldHeight * 0.5f, 0), Vector3(0, 1, 0));
}

void SceneBase2D::UpdateInputs(double dt)
{
	if (KeyboardController::GetInstance()->IsKeyDown('1'))
		glEnable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('2'))
		glDisable(GL_CULL_FACE);
	if (KeyboardController::GetInstance()->IsKeyDown('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (KeyboardController::GetInstance()->IsKeyDown('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void SceneBase2D::Update(double dt)
{
	camera->Update(dt);
	// Update our entities
	entityManager->Update(dt);

	//if (KeyboardController::GetInstance()->IsKeyDown('5'))
	//{
	//	lights[0]->type = Light::LIGHT_POINT;
	//}
	//else if (KeyboardController::GetInstance()->IsKeyDown('6'))
	//{
	//	lights[0]->type = Light::LIGHT_DIRECTIONAL;
	//}
	//else if (KeyboardController::GetInstance()->IsKeyDown('7'))
	//{
	//	lights[0]->type = Light::LIGHT_SPOT;
	//}

	//if (KeyboardController::GetInstance()->IsKeyDown('I'))
	//	lights[0]->position.z -= (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('K'))
	//	lights[0]->position.z += (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('J'))
	//	lights[0]->position.x -= (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('L'))
	//	lights[0]->position.x += (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('O'))
	//	lights[0]->position.y -= (float)(10.f * dt);
	//if (KeyboardController::GetInstance()->IsKeyDown('P'))
	//	lights[0]->position.y += (float)(10.f * dt);

	// <THERE>

	//GraphicsManager::GetInstance()->UpdateLights(dt);
}

void SceneBase2D::Render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	float halfOrthoWidth = orthoWidth * 0.5f;
	float halfOrthoHeight = orthoHeight * 0.5f;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfOrthoWidth, halfOrthoWidth, -halfOrthoHeight, halfOrthoHeight, -1000, 1000);
	GraphicsManager::GetInstance()->AttachCamera(camera);
	entityManager->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(
		0, canvasWidth,		// minX - maxX
		0, canvasHeight,	// minY - maxY
		-10, 10);											// minZ - maxZ
	GraphicsManager::GetInstance()->DetachCamera();
	entityManager->RenderUI();
}

void SceneBase2D::Exit()
{
	// Detach camera from other entities
	GraphicsManager::GetInstance()->DetachCamera();

	// Delete the lights
	//delete lights[0];
	//delete lights[1];
}
