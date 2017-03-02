#include "SceneTutorial.h"
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
#include "..\Physics\Physics.h"
#include "AnimationManager.h"
#include "GameInfo.h"

#include "..\ScrollingCamera.h"

#include "..\PlayerInfo\playerRangeEntity.h"
#include "../PlayerInfo/PlayerMeleeEntity.h"
#include "..\ScrollingEntity.h"
#include "..\FloorTiles\FloorTile.h"
#include "..\SpriteEntity.h"
#include "..\FloorTiles\FloorTilesManager.h"
#include "..\SP4\Wall.h"
#include "../SP4/HealthUI.h"

#include "..\Enemy\Floopy.h"
#include "../Enemy\Bitey.h"

SceneTutorial::SceneTutorial():
goal(nullptr)
{
	this->entityManager = EntityManager::GetInstance();
}

SceneTutorial::~SceneTutorial()
{
}

void SceneTutorial::Init()
{
	SceneBase2D::Init();

	if (camera)
		delete camera;
	camera = new ScrollingCamera();

	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("floor", Color(0.5f, 0.5f, 0.5f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("floor")->textureID = LoadTGA("Image//sceneTutorial_bg.tga");

	MeshBuilder::GetInstance()->GenerateQuad("quad22", Color(0.5f, 0.5f, 0.5f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad22")->textureID = LoadTGA("Image//transTexture.tga");

	MeshBuilder::GetInstance()->GenerateCircle("circle", Color(0.5f, 0.5f, 0.5f), 36, 1.0f);
	MeshBuilder::GetInstance()->GetMesh("circle")->textureID = LoadTGA("Image//transTexture.tga");

	MeshBuilder::GetInstance()->GenerateQuad("trigger", Color(0.8f, 0.3f, 0.3f), 1.f);
	MeshBuilder::GetInstance()->GenerateCircle("invul", Color(0.5f, 0.2f, 0.2f), 36, 1.0f);
	MeshBuilder::GetInstance()->GenerateCircle("goal", Color(0.0f, 0.8f, 0.1f), 36, 1.0f);

	MeshBuilder::GetInstance()->GenerateSprite("floopy", 1, 4, 2.f);
	MeshBuilder::GetInstance()->GetMesh("floopy")->textureID = LoadTGA("Image//floopy.tga");
	AnimationManager::GetInstance("floopy")->AddAnimation("move", new Animation(0, 3, 0.7f));

	MeshBuilder::GetInstance()->GenerateSprite("bitey", 1, 6, 2.f);
	MeshBuilder::GetInstance()->GetMesh("bitey")->textureID = LoadTGA("Image//bitey.tga");
	AnimationManager::GetInstance("bitey")->AddAnimation("move", new Animation(0, 3));
	AnimationManager::GetInstance("bitey")->AddAnimation("attack_downwards", new Animation(1, 1));
	AnimationManager::GetInstance("bitey")->AddAnimation("attack_right", new Animation(4, 4));
	AnimationManager::GetInstance("bitey")->AddAnimation("attack_left", new Animation(5, 5));

	orthoWidth = 110;
	orthoHeight = orthoWidth * 1080 / 1920;//*(Application::GetInstance().GetWindowHeight() / (float)Application::GetInstance().GetWindowWidth());
	worldWidth = 120.0f;
	worldHeight = 120.0f;
	float halfWorldWidth = worldWidth * 0.5f;
	float halfWorldHeight = worldHeight * 0.5f;

	ScrollingCamera* cammy = dynamic_cast<ScrollingCamera*>(camera);
	if (cammy)
	{
		cammy->SetWorldMin(Vector3(0, 0, 0));
		cammy->SetWorldMax(Vector3(worldWidth, worldHeight, 0));
		cammy->SetMinOrthoSize(Vector3(orthoWidth, orthoHeight, 0));
		cammy->SetMaxOrthoSize(Vector3(worldWidth, worldHeight, 0));
	}
	camera->Init(Vector3(halfWorldWidth, halfWorldHeight, 1), Vector3(halfWorldWidth, halfWorldHeight, 0), Vector3(0, 1, 0));

	{//for floor
		floor = Create::floorTileEntity(entityManager, "floor", Vector3(halfWorldWidth, halfWorldHeight, -1.0f), Vector3(worldWidth, worldHeight, 1));
		ScrollingCamera* cammy = dynamic_cast<ScrollingCamera*>(camera);
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			float yPos = floor->GetPosition().y - (floor->GetScale().y * 0.5f);
			q->GetCharacter()->SetPosition(Vector3(worldWidth * 0.2f, worldHeight * 0.11f, 0));
			q->GetCharacter()->SetSpawnPoint(Vector3(worldWidth * 0.2f, worldHeight * 0.11f, 0));
			entityManager->GetInstance()->RemoveEntity(q->GetCharacter());
			entityManager->AddEntity(q->GetCharacter());

			if (cammy)
				cammy->AddToWatch(q->GetCharacter());
		}
	}
	GameInfo::GetInstance()->SetHealth(1);
	Create::healthUIEntity(entityManager, "heart", Vector3((canvasWidth * 0.1f) * 1080 / 1920, canvasHeight * 0.1f, 1), Vector3(canvasWidth, canvasHeight, 0));

	{//Border
		//btm wall
		Create::squareWall(entityManager, "quad22", Vector3(halfWorldWidth, worldHeight * 0.03f, 0.0f), Vector3(worldWidth, worldHeight * 0.06f, 1.0f));
		//top wall
		Create::squareWall(entityManager, "quad22", Vector3(halfWorldWidth, worldHeight * 0.875f, 0.0f), Vector3(worldWidth, worldHeight * 0.25f, 1.0f));
		//left wall
		Create::squareWall(entityManager, "quad22", Vector3(worldWidth * 0.075f, halfWorldHeight, 0.0f), Vector3(worldWidth * 0.15f, worldHeight, 1.0f));
		//right wall
		Create::squareWall(entityManager, "quad22", Vector3(worldWidth * 0.925f, halfWorldHeight, 0.0f), Vector3(worldWidth * 0.15f, worldHeight, 1.0f));

		{//First Obstacles
			Create::squareWall(entityManager, "quad22", Vector3(worldWidth * 0.25f, worldHeight * 0.2f, 0.0f), Vector3(worldWidth * 0.5f, worldHeight * 0.05f, 1.0f));
			Create::roundWall(entityManager, "circle", Vector3(worldWidth * 0.8f, worldHeight * 0.2f, 0), Vector3(worldWidth * 0.125f, worldWidth * 0.125f, 1.0f));
			Create::squareWall(entityManager, "quad22", Vector3(worldWidth * 0.75f, worldHeight * 0.35f, 0.0f), Vector3(worldWidth * 0.5f, worldHeight * 0.05f, 1.0f));
			Create::squareWall(entityManager, "quad22", Vector3(worldWidth * 0.35f, worldHeight * 0.4f, 0.0f), Vector3(worldWidth * 0.3f, worldHeight * 0.05f, 1.0f));

			Create::floopyEntity(entityManager, "floopy", 0.0f, 0.0f, 1.0f, 1.5f, 1.0f, Vector3(worldWidth * 0.55f, worldHeight * 0.11f, 0.0f), Vector3(3, 3, 1));
			Create::floopyEntity(entityManager, "floopy", 0.0f, 0.0f, 1.0f, 1.5f, 1.0f, Vector3(worldWidth * 0.45f, worldHeight * 0.35f, 0.0f), Vector3(3, 3, 1));
			Create::floopyEntity(entityManager, "floopy", 0.0f, 0.0f, 1.0f, 1.5f, 1.0f, Vector3(worldWidth * 0.40f, worldHeight * 0.35f, 0.0f), Vector3(3, 3, 1));
			Create::floopyEntity(entityManager, "floopy", 0.0f, 0.0f, 1.0f, 1.5f, 1.0f, Vector3(worldWidth * 0.35f, worldHeight * 0.35f, 0.0f), Vector3(3, 3, 1));
		}
		{//Second Obstacles
			enemiesSpawned = false;
			spawnTrigger = Create::Entity(entityManager, "trigger", Vector3(worldWidth * 0.2f, worldHeight * 0.47f, 0.0f), Vector3(worldWidth * 0.2f, worldHeight * 0.1f, 1.0f));
			spawnTrigger->SetShouldRender(false);
			spawnTrigger->SetColliderType(Collider::COLLIDER_BOX);
			spawnTrigger->SetColliderSize(spawnTrigger->GetScale());
			Create::squareWall(entityManager, "quad22", Vector3(worldWidth * 0.2f, worldHeight * 0.56f, 0.0f), Vector3(worldWidth * 0.2f, worldHeight * 0.05f, 1.0f));
		}
		{//Goal
			Create::squareWall(entityManager, "quad22", Vector3(worldWidth * 0.55f, worldHeight * 0.495f, 0.0f), Vector3(worldWidth * 0.05f, worldHeight * 0.25f, 1.0f));
			goal = Create::Entity(entityManager, "goal", Vector3(worldWidth * 0.8f, worldHeight * 0.425f, 0.0f), Vector3(worldWidth * 0.05f, worldWidth * 0.05f, 1));
			goal->SetColliderType(Collider::COLLIDER_SPHERE);
			goal->SetColliderSize(goal->GetScale());
		}
	}
	
	pauseGame = true;
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glClearColor(0, 0.0f, 0.1f, 1);
}

void SceneTutorial::UpdateInputs(double dt)
{
	pauseGame = false;
	SceneBase2D::UpdateInputs(dt);
	float mouseX, mouseY;
	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
	mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
	mouseX = (mouseX / Application::GetInstance().GetWindowWidth()) * orthoWidth + (camera->GetPosition().y - (orthoHeight*0.5f));
	mouseY = (mouseY / Application::GetInstance().GetWindowHeight()) * orthoHeight + (camera->GetPosition().x - (orthoWidth*0.5f));
	Vector3 mousePos(mouseX, mouseY, 0);

	for (auto q : GameInfo::GetInstance()->GetPlayersList())
		q->UpdateInputs(dt);

	if (KeyboardController::GetInstance()->IsKeyPressed('Y'))
	{
		for (auto q : FloorTilesManager::GetInstance()->GetList())
		{
			q->SetShouldRender(!q->GetShouldRender());
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		pauseGame = true;
		SceneManager::GetInstance()->OverlayScene("PauseState");
	}
}

void SceneTutorial::Update(double dt)
{
	if (pauseGame)
		return;
	//orthoWidth = orthoHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());
	//orthoHeight = orthoWidth * (Application::GetInstance().GetWindowHeight() / (float)Application::GetInstance().GetWindowWidth());
	//ScrollingCamera* cammy = dynamic_cast<ScrollingCamera*>(camera);
	//orthoWidth = cammy->GetOrthoSize().x;
	//orthoWidth = cammy->GetOrthoSize().x;
	SceneBase2D::Update(dt);
	{//Handle Collisions
		std::list<EntityBase*>::iterator it;
		std::list<EntityBase*>::iterator it2;
		std::list<EntityBase*> list = entityManager->GetList();
		for (it = list.begin(); it != list.end(); ++it)
		{
			if ((*it)->GetColliderIsActive() == false)
				continue;
			if ((*it)->GetColliderType() == Collider::COLLIDER_NONE || (*it)->GetColliderType() == Collider::NUM_COLLIDER_TYPE)
				continue;

			for (it2 = it; it2 != list.end(); ++it2)
			{
				if ((*it)->GetColliderIsActive() == false)
					continue;
				if (it == it2)
					continue;
				if ((*it2)->GetColliderType() == Collider::COLLIDER_NONE || (*it2)->GetColliderType() == Collider::NUM_COLLIDER_TYPE)
					continue;

				if (Physics::CheckCollision(*it, *it2, dt))
				{
					(*it)->CollisionResponse(*it2, dt);
					(*it2)->CollisionResponse(*it, dt);
				}
			}
		}
	}
	entityManager->HandleOutOfBounds(Vector3(), Vector3(worldWidth, worldHeight, 0), dt);

	if (GameInfo::GetInstance()->GetHealth() <= 0)
	{
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			q->GetCharacter()->SetHasDropped(true);
		}
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			q->GetCharacter()->Respawn();
		}

		GameInfo::GetInstance()->SetHealth(1);
	}

	{//check spawn trigger
		if (!enemiesSpawned)
		{
			for (auto q : GameInfo::GetInstance()->GetPlayersList())
			{
				if (Physics::CheckCollision(q->GetCharacter(), spawnTrigger, dt))
				{
					enemiesSpawned = true;

					{//Spawn enemies
						Bitey* bitey = Create::biteyEntity(entityManager, "bitey", 50.0f, 5.0f, 1.0f, 2.0f, 2.0f, Vector3(worldWidth * 0.32f, worldHeight * 0.6f, 0.0f), Vector3(3, 3, 1));
						bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, worldHeight, 0));
						bitey->AddWaypoint(bitey->GetPosition());

						bitey = Create::biteyEntity(entityManager, "bitey", 50.0f, 5.0f, 1.0f, 2.0f, 2.0f, Vector3(worldWidth * 0.35f, worldHeight * 0.6f, 0.0f), Vector3(3, 3, 1));
						bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, worldHeight, 0));
						bitey->AddWaypoint(Vector3(worldWidth * 0.34f, worldHeight * 0.7f, 0.0f));
						bitey->AddWaypoint(Vector3(worldWidth * 0.40f, worldHeight * 0.7f, 0.0f));

						bitey = Create::biteyEntity(entityManager, "bitey", 50.0f, 5.0f, 1.0f, 2.0f, 2.0f, Vector3(worldWidth * 0.37f, worldHeight * 0.6f, 0.0f), Vector3(3, 3, 1));
						bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, worldHeight, 0));
						bitey->AddWaypoint(Vector3(worldWidth * 0.40f, worldHeight * 0.7f, 0.0f));
						bitey->AddWaypoint(Vector3(worldWidth * 0.46f, worldHeight * 0.7f, 0.0f));

						bitey = Create::biteyEntity(entityManager, "bitey", 50.0f, 5.0f, 1.0f, 2.0f, 2.0f, Vector3(worldWidth * 0.37f, worldHeight * 0.6f, 0.0f), Vector3(3, 3, 1));
						bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, worldHeight, 0));
						bitey->AddWaypoint(Vector3(worldWidth * 0.46f, worldHeight * 0.7f, 0.0f));
						bitey->AddWaypoint(Vector3(worldWidth * 0.52f, worldHeight * 0.7f, 0.0f));
					}

					break;
				}
			}
		}
	}
	{//check goal reached
		int numPlayersEnteredGoal = 0;
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			if (Physics::CheckCollision(q->GetCharacter(), goal, dt))
				numPlayersEnteredGoal++;
		}
		if (numPlayersEnteredGoal >= GameInfo::GetInstance()->GetPlayersList().size())
			SceneManager::GetInstance()->SetActiveScene("GameModeSelectionState");
	}
}

void SceneTutorial::Render()
{
	SceneBase2D::Render();
}

void SceneTutorial::Exit()
{
	SceneBase2D::Exit();

	entityManager->ClearList();
}