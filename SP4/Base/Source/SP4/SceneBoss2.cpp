#include "SceneBoss2.h"
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

SceneBoss2::SceneBoss2()
{
	this->entityManager = EntityManager::GetInstance();
}

SceneBoss2::~SceneBoss2()
{
}

void SceneBoss2::Init()
{
	SceneBase2D::Init();

	if (camera)
		delete camera;
	camera = new ScrollingCamera();

	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("floor", Color(0.5f, 0.5f, 0.5f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("floor")->textureID = LoadTGA("Image//bossmap.tga");
	MeshBuilder::GetInstance()->GenerateQuad("water", Color(0.2f, 0.2f, 1.0f), 1.f);
	//MeshBuilder::GetInstance()->GetMesh("water")->textureID = LoadTGA("Image//bossmap_water.tga");

	orthoWidth = 80;
	orthoHeight = orthoWidth * 1080 / 1920;//*(Application::GetInstance().GetWindowHeight() / (float)Application::GetInstance().GetWindowWidth());
	worldWidth = 110.0f;
	worldHeight = 80.0f;
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
		floor = Create::floorTileEntity(entityManager, "floor", Vector3(halfWorldWidth, ((worldHeight - 25.0f) * 0.5f) + 5.0f, -1.0f), Vector3(worldWidth - 10.0f, worldHeight - 25.0f, 0));
		ScrollingCamera* cammy = dynamic_cast<ScrollingCamera*>(camera);
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			float yPos = floor->GetPosition().y - (floor->GetScale().y * 0.5f);
			q->GetCharacter()->SetPosition(Vector3(halfWorldWidth, 10.0f + q->GetCharacter()->GetScale().y, 0));
			q->GetCharacter()->SetSpawnPoint(Vector3(halfWorldWidth, 10.0f + q->GetCharacter()->GetScale().y, 0));
			entityManager->GetInstance()->RemoveEntity(q->GetCharacter());
			entityManager->AddEntity(q->GetCharacter());

			if (cammy)
				cammy->AddToWatch(q->GetCharacter());
		}
		Create::Entity(entityManager, "water", Vector3(halfWorldWidth, ((worldHeight - 25.0f) * 0.5f) + 5.0f, -3.0f), Vector3(worldWidth - 10.0f, worldHeight - 25.0f, 0));
	}
	{//Create reflection for players
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			PlayerRangeEntity* rangePlayer = dynamic_cast<PlayerRangeEntity*>(q->GetCharacter());
			PlayerMeleeEntity* meleePlayer = dynamic_cast<PlayerMeleeEntity*>(q->GetCharacter());

			if (rangePlayer)
			{
				reflectionMap[q] = Create::Sprite3DObject(entityManager, "player_staff_ripple", Vector3(), q->GetCharacter()->GetScale());
			}
			else if (meleePlayer)
			{
				reflectionMap[q] = Create::Sprite3DObject(entityManager, "player_shield_ripple", Vector3(), q->GetCharacter()->GetScale());
			}
		}
	}

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glClearColor(0, 0.0f, 0.1f, 1);
}

void SceneBoss2::UpdateInputs(double dt)
{
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
}

void SceneBoss2::Update(double dt)
{
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

	{//Update reflections
		for (auto q : reflectionMap)
		{
			Vector3 pos = q.first->GetCharacter()->GetPosition();
			pos.y -= (q.first->GetCharacter()->GetScale().y * 2.0f + 0.018f);
			pos.z = -2.0f;
			q.second->animationPlayer.m_currentFrame = q.first->GetCharacter()->animationPlayer.m_currentFrame;
			q.second->SetPosition(pos);
			if (q.first->GetCharacter()->GetHasDropped() ||
				q.second->GetPosition().y - (q.second->GetScale().y) < floor->GetPosition().y - floor->GetScale().y * 0.5f)
				q.second->SetShouldRender(false);
			else
				q.second->SetShouldRender(true);
		}
	}
}

void SceneBoss2::Render()
{
	SceneBase2D::Render();
}

void SceneBoss2::Exit()
{
	SceneBase2D::Exit();

	entityManager->ClearList();
}