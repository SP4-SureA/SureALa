#include "GameModeSelectionState.h"
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
#include "AnimationManager.h"
#include "..\SP4\GameInfo.h"

GameModeSelectionState::GameModeSelectionState() :
currentSelection(STORY)
{
	entityManager = new EntityManager();
}

GameModeSelectionState::~GameModeSelectionState()
{
	if (entityManager)
	{
		entityManager->ClearList();
		delete entityManager;
		entityManager = NULL;
	}
}

void GameModeSelectionState::Init()
{
	SceneBase2D::Init();

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("BACKGROUND", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("BACKGROUND")->textureID = LoadTGA("Image//bg_gamemode_select.tga");

    MeshBuilder::GetInstance()->GenerateQuad("BUTTON_IMG", Color(0, 0, 0), 1.0f);

    MeshBuilder::GetInstance()->GenerateQuad("SELECTION_ICON", Color(1, 1, 1), 1.0f);
    MeshBuilder::GetInstance()->GetMesh("SELECTION_ICON")->textureID = LoadTGA("Image//selection_icon.tga");

	orthoHeight = (float)Application::GetInstance().GetWindowHeight();
	orthoWidth = (float)Application::GetInstance().GetWindowWidth();
	worldHeight = orthoHeight;
	worldWidth = orthoHeight;

	float halfWindowWidth = orthoWidth * 0.5f;
	float halfWindowHeight = orthoHeight * 0.5f;

	camera->Init(Vector3(halfWindowWidth, halfWindowHeight, 1), Vector3(halfWindowWidth, halfWindowHeight, 0), Vector3(0, 1, 0));

    background = Create::Entity(entityManager, "BACKGROUND", Vector3(halfWindowWidth, halfWindowHeight, -1.0f), Vector3(orthoWidth, orthoHeight, 1));

    { // Buttons
        storyBtn = Create::button(entityManager, "BUTTON_IMG", Vector3(orthoWidth * 0.53f, orthoHeight * 0.36f, 0.0f), Vector3(orthoWidth * 0.3f, orthoHeight * 0.08f, 0.0f));
        endlessBtn = Create::button(entityManager, "BUTTON_IMG", Vector3(orthoWidth * 0.53f, orthoHeight * 0.27f, 0.0f), Vector3(orthoWidth * 0.3f, orthoHeight * 0.08f, 0.0f));
        exitBtn = Create::button(entityManager, "BUTTON_IMG", Vector3(orthoWidth * 0.53f, orthoHeight * 0.12f, 0.0f), Vector3(orthoWidth * 0.3f, orthoHeight * 0.08f, 0.0f));
    
    
        storyBtn->SetShouldRender(false);
        endlessBtn->SetShouldRender(false);
        exitBtn->SetShouldRender(false);
    }

    selectionIcon = Create::button(entityManager, "SELECTION_ICON", Vector3(orthoWidth * 0.52f, orthoHeight * 0.45f, 0.0f), Vector3(orthoWidth * 0.25f, orthoHeight * 0.08f, 0.0f));
}

void GameModeSelectionState::UpdateInputs(double dt)
{
    UpdateKeyboard();
    UpdateMouse();
}

void GameModeSelectionState::UpdateMouse()
{
    float mouseX, mouseY;
    MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
    mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
    mouseX *= canvasWidth / Application::GetInstance().GetWindowWidth();
    mouseY *= canvasHeight / Application::GetInstance().GetWindowHeight();
    Vector3 mousePos(mouseX, mouseY, 0);

    Button* currentButton = nullptr;

    if (storyBtn->CheckCollision(mousePos))
    {
        selectionIcon->SetPosition(storyBtn->GetPosition());
        currentSelection = STORY;
        currentButton = storyBtn;
    }
    if (endlessBtn->CheckCollision(mousePos))
    {
        selectionIcon->SetPosition(endlessBtn->GetPosition());
        currentSelection = ENDLESS;
        currentButton = endlessBtn;
    }
    if (exitBtn->CheckCollision(mousePos))
    {
        selectionIcon->SetPosition(exitBtn->GetPosition());
        currentSelection = EXIT;
        currentButton = exitBtn;
    }

    if (MouseController::GetInstance()->IsButtonPressed(MouseController::LMB) && currentButton)
    {
        switch (currentSelection)
        {
        case STORY:
			GameInfo::GetInstance()->SetGameMode(GameInfo::GAMEMODE_STORY);
            SceneManager::GetInstance()->SetActiveScene("SelectionState");
            break;
        case ENDLESS:
			GameInfo::GetInstance()->SetGameMode(GameInfo::GAMEMODE_ENDLESS);
            SceneManager::GetInstance()->SetActiveScene("SelectionState");
            break;
        case EXIT:
            SceneManager::GetInstance()->SetActiveScene("MenuState");
            break;
        default:
            break;
        }
    }
}

void GameModeSelectionState::UpdateKeyboard()
{
    switch (currentSelection)
    {
    case STORY:
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
        {
            currentSelection = EXIT;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
        {
            currentSelection = ENDLESS;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
        {
			GameInfo::GetInstance()->SetGameMode(GameInfo::GAMEMODE_STORY);
			SceneManager::GetInstance()->SetActiveScene("SelectionState");
        }
        selectionIcon->SetPosition(storyBtn->GetPosition());
        break;
    case ENDLESS:
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
        {
            currentSelection = STORY;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
        {
            currentSelection = EXIT;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
        {
			GameInfo::GetInstance()->SetGameMode(GameInfo::GAMEMODE_TUTORIAL);
			SceneManager::GetInstance()->SetActiveScene("SelectionState");
        }
        selectionIcon->SetPosition(endlessBtn->GetPosition());
        break;
    case EXIT:
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP))
        {
            currentSelection = ENDLESS;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN))
        {
            currentSelection = STORY;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
        {
			SceneManager::GetInstance()->SetActiveScene("MenuState");
        }
        selectionIcon->SetPosition(exitBtn->GetPosition());
        break;
    default:
        break;
    }
}

void GameModeSelectionState::Update(double dt)
{
}

void GameModeSelectionState::Render()
{
	SceneBase2D::Render();
}

void GameModeSelectionState::Exit()
{
	SceneBase2D::Exit();
	
	MeshBuilder::GetInstance()->RemoveMesh("BACKGROUND");
    MeshBuilder::GetInstance()->RemoveMesh("BUTTON_IMG");
    MeshBuilder::GetInstance()->RemoveMesh("SELECTION_ICON");

	if (entityManager)
	{
		entityManager->ClearList();
	}

	if (background)
		delete background;
}