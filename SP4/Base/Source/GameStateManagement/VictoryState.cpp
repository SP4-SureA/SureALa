#include "VictoryState.h"
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

VictoryState::VictoryState()
{
	entityManager = new EntityManager();
}

VictoryState::~VictoryState()
{
	if (entityManager)
	{
		entityManager->ClearList();
		delete entityManager;
		entityManager = NULL;
	}
}

void VictoryState::Init()
{
	SceneBase2D::Init();

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("BACKGROUND", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("BACKGROUND")->textureID = LoadTGA("Image//bg_victory.tga");

	orthoHeight = (float)Application::GetInstance().GetWindowHeight();
	orthoWidth = (float)Application::GetInstance().GetWindowWidth();
	worldHeight = orthoHeight;
	worldWidth = orthoHeight;

	float halfWindowWidth = orthoWidth * 0.5f;
	float halfWindowHeight = orthoHeight * 0.5f;

	camera->Init(Vector3(halfWindowWidth, halfWindowHeight, 1), Vector3(halfWindowWidth, halfWindowHeight, 0), Vector3(0, 1, 0));

    background = Create::Entity(entityManager, "BACKGROUND", Vector3(halfWindowWidth, halfWindowHeight, -1.0f), Vector3(orthoWidth, orthoHeight, 1));
}

void VictoryState::UpdateInputs(double dt)
{
    if (MouseController::GetInstance()->IsButtonPressed(VK_RETURN) || MouseController::GetInstance()->IsButtonPressed(MouseController::LMB))
    {
        SceneManager::GetInstance()->SetActiveScene("MenuState");
    }
}

void VictoryState::Update(double dt)
{
}

void VictoryState::Render()
{
	SceneBase2D::Render();
}

void VictoryState::Exit()
{
	SceneBase2D::Exit();
	
	MeshBuilder::GetInstance()->RemoveMesh("BACKGROUND");

	if (entityManager)
	{
		entityManager->ClearList();
	}

	if (background)
		delete background;
}