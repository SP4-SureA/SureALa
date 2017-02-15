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

//for tests only, to be removed
#include "..\RakNet\Client.h"
#include "..\PlayerInfo\PlayerEntityBase.h"
#include "..\RakNet\NetworkEntityManager.h"

MenuState::MenuState()
{
}

MenuState::~MenuState()
{
}

void MenuState::Init()
{
	SceneBase2D::Init();

	
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	Application::ShowCursor();

	MeshBuilder::GetInstance()->GenerateSprite("player_shield", 16, 4, 2.0f);
	MeshBuilder::GetInstance()->GetMesh("player_shield")->textureID = LoadTGA("Image//spritesheet_shield.tga");

	float animSpeed = 0.7f;
	AnimationManager::GetInstance("player_shield")->AddAnimation("right", new Animation(32, 35, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("left", new Animation(36, 39, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("up_right", new Animation(40, 43, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("up_left", new Animation(44, 47, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("up", new Animation(48, 51, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("down", new Animation(52, 55, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("down_left", new Animation(56, 59, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("down_right", new Animation(60, 63, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("shield_right", new Animation(0, 3, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("shield_left", new Animation(4, 7, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("shield_up_right", new Animation(8, 11, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("shield_up_left", new Animation(12, 15, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("shield_up", new Animation(16, 19, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("shield_down", new Animation(20, 23, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("shield_down_left", new Animation(24, 27, animSpeed));
	AnimationManager::GetInstance("player_shield")->AddAnimation("shield_down_right", new Animation(28, 31, animSpeed));

	MeshBuilder::GetInstance()->GenerateCircle("circle", Color(1, 0, 1), 36, 1);
	MeshBuilder::GetInstance()->GenerateQuad("MENUSTATE_BKGROUND", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("MENUSTATE_BKGROUND")->textureID = LoadTGA("Image//background.tga");
	MeshBuilder::GetInstance()->GenerateQuad("playBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("playBtn")->textureID = LoadTGA("Image//playBtn.tga");

	orthoHeight = (float)Application::GetInstance().GetWindowHeight();
	orthoWidth = (float)Application::GetInstance().GetWindowWidth();
	worldHeight = orthoHeight;
	worldWidth = orthoHeight;

	float halfWindowWidth = orthoWidth * 0.5f;
	float halfWindowHeight = orthoHeight * 0.5f;

	camera.Init(Vector3(halfWindowWidth, halfWindowHeight, 1), Vector3(halfWindowWidth, halfWindowHeight, 0), Vector3(0, 1, 0));

	PlayerInfo::GetInstance()->SetCharacter(Create::PlayerEntity(SceneManager::GetInstance()->GetScene("SceneScrolling")->GetEntityManager(), "player_shield", 10, 1, Vector3(50, 50, 0), Vector3(3, 3, 1)));
	//PlayerInfo::GetInstance()->SetCharacter(Create::PlayerEntity(SceneManager::GetInstance()->GetScene("SceneTest")->GetEntityManager(), "circle", 40, 5, Vector3(50, 50, 0), Vector3(3, 3, 1)));

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
			SceneManager::GetInstance()->SetActiveScene("SceneScrolling");
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('H'))
	{
		Client::GetInstance()->Host(SceneManager::GetInstance()->GetScene("SceneScrolling"));
		SceneManager::GetInstance()->SetActiveScene("SceneScrolling");
	}
	if (KeyboardController::GetInstance()->IsKeyPressed('J'))
	{
		Client::GetInstance()->Join(SceneManager::GetInstance()->GetScene("SceneScrolling"));
		SceneManager::GetInstance()->SetActiveScene("SceneScrolling");
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
	MeshBuilder::GetInstance()->RemoveMesh("playBtn");
	MeshBuilder::GetInstance()->RemoveMesh("settingsBtn");

	entityManager->ClearList();
}