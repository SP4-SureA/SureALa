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
#include "AnimationManager.h"

MenuState::MenuState() :
currentSelection(PLAY)
{
	entityManager = new EntityManager();
}

MenuState::~MenuState()
{
	if (entityManager)
	{
		entityManager->ClearList();
		delete entityManager;
		entityManager = NULL;
	}
}

void MenuState::Init()
{
	SceneBase2D::Init();

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//bg_mainmenu.tga");

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

    background = Create::Entity(entityManager, "MENUSTATE_BKGROUND", Vector3(halfWindowWidth, halfWindowHeight, -1.0f), Vector3(orthoWidth, orthoHeight, 1));

    { // Buttons
        playBtn = Create::button(entityManager, "BUTTON_IMG", Vector3(orthoWidth * 0.53f, orthoHeight * 0.45f, 0.0f), Vector3(orthoWidth * 0.3f, orthoHeight * 0.08f, 0.0f));
        helpBtn = Create::button(entityManager, "BUTTON_IMG", Vector3(orthoWidth * 0.53f, orthoHeight * 0.36f, 0.0f), Vector3(orthoWidth * 0.3f, orthoHeight * 0.08f, 0.0f));
        optionBtn = Create::button(entityManager, "BUTTON_IMG", Vector3(orthoWidth * 0.53f, orthoHeight * 0.27f, 0.0f), Vector3(orthoWidth * 0.3f, orthoHeight * 0.08f, 0.0f));
        exitBtn = Create::button(entityManager, "BUTTON_IMG", Vector3(orthoWidth * 0.53f, orthoHeight * 0.12f, 0.0f), Vector3(orthoWidth * 0.3f, orthoHeight * 0.08f, 0.0f));
    
    
        playBtn->SetShouldRender(false);
        helpBtn->SetShouldRender(false);
        optionBtn->SetShouldRender(false);
        exitBtn->SetShouldRender(false);
    }

    selectionIcon = Create::button(entityManager, "SELECTION_ICON", Vector3(orthoWidth * 0.52f, orthoHeight * 0.45f, 0.0f), Vector3(orthoWidth * 0.3f, orthoHeight * 0.08f, 0.0f));
}

void MenuState::UpdateInputs(double dt)
{
    UpdateKeyboard();
    UpdateMouse();
}

void MenuState::UpdateMouse()
{
    float mouseX, mouseY;
    MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
    mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
    mouseX *= canvasWidth / Application::GetInstance().GetWindowWidth();
    mouseY *= canvasHeight / Application::GetInstance().GetWindowHeight();
    Vector3 mousePos(mouseX, mouseY, 0);

    Button* currentButton = nullptr;

    if (playBtn->CheckCollision(mousePos))
    {
        selectionIcon->SetPosition(playBtn->GetPosition());
        currentSelection = PLAY;
        currentButton = playBtn;
    }
    if (helpBtn->CheckCollision(mousePos))
    {
        selectionIcon->SetPosition(helpBtn->GetPosition());
        currentSelection = HELP;
        currentButton = helpBtn;
    }
    if (optionBtn->CheckCollision(mousePos))
    {
        selectionIcon->SetPosition(optionBtn->GetPosition());
        currentSelection = OPTION;
        currentButton = optionBtn;
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
        case PLAY:
            SceneManager::GetInstance()->SetActiveScene("GameModeSelectionState");
            break;
        case HELP:
            //SceneManager::GetInstance()->SetActiveScene("SelectionState");
            break;
        case OPTION:
            //SceneManager::GetInstance()->SetActiveScene("SelectionState");
            break;
        case EXIT:
            Application::GetInstance().Close();
            break;
        default:
            break;
        }
    }
}

void MenuState::UpdateKeyboard()
{
    switch (currentSelection)
    {
    case PLAY:
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) || KeyboardController::GetInstance()->IsKeyPressed('W'))
        {
            currentSelection = EXIT;
        }
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) || KeyboardController::GetInstance()->IsKeyPressed('S'))
        {
            currentSelection = HELP;
        }
		if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN) || KeyboardController::GetInstance()->IsKeyPressed(' '))
        {
            SceneManager::GetInstance()->SetActiveScene("GameModeSelectionState");
        }
        selectionIcon->SetPosition(playBtn->GetPosition());
        break;
    case HELP:
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) || KeyboardController::GetInstance()->IsKeyPressed('W'))
        {
            currentSelection = PLAY;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) || KeyboardController::GetInstance()->IsKeyPressed('S'))
        {
            currentSelection = OPTION;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
        {
           
        }
        selectionIcon->SetPosition(helpBtn->GetPosition());
        break;
    case OPTION:
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) || KeyboardController::GetInstance()->IsKeyPressed('W'))
        {
            currentSelection = HELP;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) || KeyboardController::GetInstance()->IsKeyPressed('S'))
        {
            currentSelection = EXIT;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
        {
            
        }
        selectionIcon->SetPosition(optionBtn->GetPosition());
        break;
    case EXIT:
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_UP) || KeyboardController::GetInstance()->IsKeyPressed('W'))
        {
            currentSelection = OPTION;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_DOWN) || KeyboardController::GetInstance()->IsKeyPressed('S'))
        {
            currentSelection = PLAY;
        }
        if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
        {
            Application::GetInstance().Close();
        }
        selectionIcon->SetPosition(exitBtn->GetPosition());
        break;
    default:
        break;
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
	
	MeshBuilder::GetInstance()->RemoveMesh("MENUSTATE_BKGROUND");
    MeshBuilder::GetInstance()->RemoveMesh("BUTTON_IMG");
    MeshBuilder::GetInstance()->RemoveMesh("SELECTION_ICON");

	if (entityManager)
	{
		entityManager->ClearList();
	}

	if (playBtn)
		delete playBtn;
	if (background)
		delete background;
}