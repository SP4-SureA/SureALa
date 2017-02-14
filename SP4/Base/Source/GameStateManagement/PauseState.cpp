#include "PauseState.h"
#include "GL\glew.h"

#include "..\Application.h"
#include "LoadTGA.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "..\TextEntity.h"
#include "RenderHelper.h"

#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"

PauseState::PauseState()
{
}

PauseState::~PauseState()
{
}

void PauseState::Init()
{
	SceneBase2D::Init();
	Application::ShowCursor();

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);

	MeshBuilder::GetInstance()->GenerateQuad("PAUSESTATE_BKGROUND", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("PAUSESTATE_BKGROUND")->textureID = LoadTGA("Image//background2.tga");
	//MeshBuilder::GetInstance()->GenerateQuad("settingsBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	//MeshBuilder::GetInstance()->GetMesh("settingsBtn")->textureID = LoadTGA("Image//settingsBtn.tga");
	MeshBuilder::GetInstance()->GenerateQuad("menuBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("menuBtn")->textureID = LoadTGA("Image//menuBtn.tga");
	MeshBuilder::GetInstance()->GenerateQuad("backBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("backBtn")->textureID = LoadTGA("Image//backBtn.tga");
	
	orthoHeight = (float)Application::GetInstance().GetWindowHeight();
	orthoWidth = (float)Application::GetInstance().GetWindowWidth();
	worldHeight = orthoHeight;
	worldWidth = orthoHeight;

	float halfWindowWidth = orthoWidth * 0.5f;
	float halfWindowHeight = orthoHeight * 0.5f;

	camera.Init(Vector3(halfWindowWidth, halfWindowHeight, 1), Vector3(halfWindowWidth, halfWindowHeight, 0), Vector3(0, 1, 0));

	background = Create::Entity(entityManager, "PAUSESTATE_BKGROUND", Vector3(halfWindowWidth, halfWindowHeight, 0.0f), Vector3(halfWindowWidth * 1.5f, halfWindowHeight * 1.5f, 1.0f));
	backBtn = Create::button(entityManager, "backBtn", Vector3(worldWidth * 0.3f, worldHeight * 0.75f, 1.0f), Vector3(150, 60, 1));
	menuBtn = Create::button(entityManager, "menuBtn", Vector3(halfWindowWidth, halfWindowHeight * 0.4f, 1.0f), Vector3(200, 60, 1));
}

void PauseState::UpdateInputs(double dt)
{
	float mouseX, mouseY;
	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
	mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
	Vector3 mousePos(mouseX, mouseY, 0);

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		if (backBtn->CheckCollision(mousePos))
		{
			SceneManager::GetInstance()->ExitOverlayScene();
		}
		else if (menuBtn->CheckCollision(mousePos))
		{
			SceneManager::GetInstance()->SetActiveScene("MenuState");
		}
	}
}

void PauseState::Update(double dt)
{
}

void PauseState::Render()
{
	SceneBase2D::Render();
}

void PauseState::Exit()
{
	SceneBase2D::Exit();

	entityManager->ClearList();
}