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
	entityManager = new EntityManager();
	for (int i = 0; i < BTN_END; ++i)
		buttons[i] = NULL;
}

PauseState::~PauseState()
{
	if (entityManager)
	{
		entityManager->ClearList();
		delete entityManager;
		entityManager = NULL;
	}
}

void PauseState::Init()
{
	SceneBase2D::Init();
	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("PAUSESTATE_BKGROUND", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("PAUSESTATE_BKGROUND")->textureID = LoadTGA("Image//pause_screen.tga");
	//MeshBuilder::GetInstance()->GenerateQuad("settingsBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	//MeshBuilder::GetInstance()->GetMesh("settingsBtn")->textureID = LoadTGA("Image//settingsBtn.tga");
	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("quad")->textureID = LoadTGA("Image//transTexture.tga");
	MeshBuilder::GetInstance()->GenerateQuad("selection", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("selection")->textureID = LoadTGA("Image//selection.tga");

	orthoHeight = 100;
	orthoWidth = orthoHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());
	worldHeight = orthoHeight;
	worldWidth = orthoWidth;
	canvasHeight = orthoHeight;
	canvasWidth = orthoWidth;

	float halfCanvasWidth = canvasWidth * 0.5f;
	float halfCanvasHeight = canvasHeight * 0.5f;
	float backgroundWidth = canvasWidth * 0.75f;
	float backgroundHeight = canvasHeight * 0.75f;

	// Create and attach the camera to the scene
	camera->Init(Vector3(worldWidth * 0.5f, worldHeight * 0.5f, 1), Vector3(worldWidth * 0.5f, worldHeight * 0.5f, 0), Vector3(0, 1, 0));

	background = Create::Entity(entityManager, "PAUSESTATE_BKGROUND", Vector3(halfCanvasWidth, halfCanvasHeight, 0.9f), Vector3(backgroundWidth, backgroundHeight, 1.0f));

	float btnWidth = backgroundWidth * 0.3f;
	float btnHeight = backgroundHeight * 0.1f;
	float yOffset = -1.0f;
	selection = Create::Entity(entityManager, "selection", Vector3(halfCanvasWidth, halfCanvasHeight + btnHeight * 2, 1.1f), Vector3(backgroundWidth * 0.45f, btnHeight, 1.0f));
	buttons[BTN_RESUME] = Create::button(entityManager, "quad", Vector3(halfCanvasWidth, yOffset+halfCanvasHeight + btnHeight * 2, 1.0f), Vector3(btnWidth, btnHeight, 1));
	buttons[BTN_HELP] = Create::button(entityManager, "quad", Vector3(halfCanvasWidth, yOffset+halfCanvasHeight + btnHeight, 1.0f), Vector3(btnWidth, btnHeight, 1));
	buttons[BTN_MENU] = Create::button(entityManager, "quad", Vector3(halfCanvasWidth, yOffset+halfCanvasHeight, 1.0f), Vector3(btnWidth, btnHeight, 1));
	
	buttons[BTN_QUIT] = Create::button(entityManager, "quad", Vector3(halfCanvasWidth, canvasHeight * 0.22f, 1.0f), Vector3(btnWidth, btnHeight, 1));
	
	for (int i = 0; i < BTN_END; ++i)
	{
		if (buttons[i])
			buttons[i]->SetShouldRender(false);
	}
	
	btnIndex = BTN_RESUME;
}

void PauseState::UpdateInputs(double dt)
{
	SceneBase2D::UpdateInputs(dt);

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		for (int i = 0; i < BTN_END; ++i)
		{
			if (buttons[i]->CheckCollision(mousePos))
			{
				switch (i)
				{
				case PauseState::BTN_RESUME:
					SceneManager::GetInstance()->ExitOverlayScene();
					break;
				case PauseState::BTN_HELP:
					break;
				case PauseState::BTN_MENU:
					SceneManager::GetInstance()->SetActiveScene("MenuState");
					break;
				case PauseState::BTN_QUIT:
					Application::Close();
					break;

				default: break;
				}

				break;
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		SceneManager::GetInstance()->ExitOverlayScene();
	}
	else if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyReleased(' '))
	{
		switch (btnIndex)
		{
		case PauseState::BTN_RESUME:
			SceneManager::GetInstance()->ExitOverlayScene();
			break;
		case PauseState::BTN_HELP:
			break;
		case PauseState::BTN_MENU:
			SceneManager::GetInstance()->SetActiveScene("MenuState");
			break;
		case PauseState::BTN_QUIT:
			Application::Close();
			break;

		default: break;
		}
	}

	if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) || KeyboardController::GetInstance()->IsKeyPressed('W'))
	{
		btnIndex = Math::Wrap(btnIndex - 1, 0, BTN_END - 1);
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) || KeyboardController::GetInstance()->IsKeyPressed('S'))
	{
		btnIndex = (btnIndex + 1) % BTN_END;
	}
}

void PauseState::Update(double dt)
{
	float mouseX, mouseY;
	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
	mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
	mouseX *= canvasWidth / Application::GetInstance().GetWindowWidth();
	mouseY *= canvasHeight / Application::GetInstance().GetWindowHeight();
	mousePos.Set(mouseX, mouseY, 0);

	float mouseDeltaX, mouseDeltaY;
	MouseController::GetInstance()->GetMouseDelta(mouseDeltaX, mouseDeltaY);

	if (mouseDeltaX || mouseDeltaY)
	{
		for (int i = 0; i < BTN_END; ++i)
		{
			if (buttons[i]->CheckCollision(mousePos))
			{
				btnIndex = i;
			}
		}
	}

	if (selection)
		selection->SetPosition(buttons[btnIndex]->GetPosition());
}

void PauseState::Render()
{
	SceneBase2D::Render();
}

void PauseState::Exit()
{
	SceneBase2D::Exit();
	
	MeshBuilder::GetInstance()->RemoveMesh("PAUSESTATE_BKGROUND");
	MeshBuilder::GetInstance()->RemoveMesh("quad");
	MeshBuilder::GetInstance()->RemoveMesh("selection");

	if (entityManager)
	{
		entityManager->ClearList();
	}
}