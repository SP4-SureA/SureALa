#include "MenuState.h"
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

MenuState::MenuState()
{
}

MenuState::~MenuState()
{
}

void MenuState::Init()
{
	SceneBase2D::Init();

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	Application::ShowCursor();

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//background.tga");
	MeshBuilder::GetInstance()->GenerateQuad("playBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("playBtn")->textureID = LoadTGA("Image//playBtn.tga");
	MeshBuilder::GetInstance()->GenerateQuad("settingsBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("settingsBtn")->textureID = LoadTGA("Image//settingsBtn.tga");

	orthoHeight = (float)Application::GetInstance().GetWindowHeight();
	orthoWidth = (float)Application::GetInstance().GetWindowWidth();
	worldHeight = orthoHeight;
	worldWidth = orthoHeight;

	float halfWindowWidth = orthoWidth * 0.5f;
	float halfWindowHeight = orthoHeight * 0.5f;

	camera.Init(Vector3(halfWindowWidth, halfWindowHeight, 1), Vector3(halfWindowWidth, halfWindowHeight, 0), Vector3(0, 1, 0));

	background = Create::Entity(entityManager, "MENUSTATE_BKGROUND", Vector3(halfWindowWidth, halfWindowHeight, -1.0f), Vector3(orthoWidth, orthoHeight, 1));
	playBtn = Create::button(entityManager, "playBtn", Vector3(halfWindowWidth, halfWindowHeight, 0.0f), Vector3(200, 60, 1));
}

void MenuState::UpdateInputs(double dt)
{
	float mouseX, mouseY;
	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
	mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
	Vector3 mousePos(mouseX, mouseY, 0);

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		if (playBtn->CheckCollision(mousePos))
		{
			SceneManager::GetInstance()->SetActiveScene("SceneTest");
		}
	}
}

void MenuState::Update(double dt)
{
}

void MenuState::Render()
{
	SceneBase2D::Render();
}

void MenuState::Exit()
{
	SceneBase2D::Exit();
	
	entityManager->ClearList();
}