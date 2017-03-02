#include "SceneBoss.h"
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

#include "../ScrollingCamera.h"
#include "..\PlayerInfo\playerRangeEntity.h"
#include "../PlayerInfo/PlayerMeleeEntity.h"
#include "..\ScrollingEntity.h"
#include "../CloudEntity/CloudFactory.h"
#include "..\FloorTiles\FloorTile.h"
#include "../Boss/Phase1/GraveStoneEntity.h"
#include "../Boss/SkyWhale.h"
#include "..\Boss\Phase1\BossMinion.h"
#include "../SpriteEntity.h"
#include "..\FloorTiles\FloorTilesManager.h"
#include "../SP4/HealthUI.h"
#include "..\SP4/EnvironmentObject.h"
#include "../SoundManager.h"

#include "HealthBarRenderer.h"

SceneBoss::SceneBoss() :
currentBossHeath(0)
{
	this->entityManager = EntityManager::GetInstance();
}

SceneBoss::~SceneBoss()
{
}

void SceneBoss::Init()
{
	SceneBase2D::Init();

    currentBossHeath = 0;

	if (camera)
		delete camera;
	camera = new ScrollingCamera();

	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("floor", Color(0.9f, 0.7f, 0.7f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("floor")->textureID = LoadTGA("Image//bossmap.tga");
	MeshBuilder::GetInstance()->GenerateQuad("water", Color(0.9f, 0.7f, 0.7f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("water")->textureID = LoadTGA("Image//bossmap_water.tga");
	//MeshBuilder::GetInstance()->GetMesh("grave_refresh_effect")->textureID = LoadTGA("Image//sprite_impact.tga");

	orthoWidth = 125;
	orthoHeight = orthoWidth * 1080 / 1920;//*(Application::GetInstance().GetWindowHeight() / (float)Application::GetInstance().GetWindowWidth());
	worldWidth = 150.0f;
	worldHeight = 150.0f;
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
	camera->Init(Vector3(halfWorldWidth, orthoHeight * 0.5f, 1), Vector3(halfWorldWidth, orthoHeight * 0.5f, 0), Vector3(0, 1, 0));

	{//for floor
		FloorTile* floor = Create::floorTileEntity(entityManager, "floor", Vector3(halfWorldWidth, halfWorldHeight, -1.0f), Vector3(worldWidth, worldHeight, 0));
		FloorTile* floor2 = Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth, halfWorldHeight, -1.0f), Vector3(worldWidth * 2, worldHeight*2, 0));
		floor2->SetShouldRender(false);
		ScrollingCamera* cammy = dynamic_cast<ScrollingCamera*>(camera);
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			float yPos = floor->GetPosition().y - (floor->GetScale().y * 0.5f);
			q->GetCharacter()->SetPosition(Vector3(halfWorldWidth, yPos + q->GetCharacter()->GetScale().y * 0.5f, 0));
			q->GetCharacter()->SetSpawnPoint(Vector3(halfWorldWidth, yPos + q->GetCharacter()->GetScale().y * 0.5f, 0));
			entityManager->GetInstance()->RemoveEntity(q->GetCharacter());
			entityManager->AddEntity(q->GetCharacter());

			if (cammy)
				cammy->AddToWatch(q->GetCharacter());
		}
		Create::Entity(entityManager, "water", Vector3(halfWorldWidth, halfWorldHeight, -3.0f), Vector3(worldWidth, worldHeight, 0));
	}

	Create::healthUIEntity(entityManager, "heart", Vector3((canvasWidth * 0.1f) * 1080 / 1920, canvasHeight * 0.1f, 1), Vector3(canvasWidth, canvasHeight, 0));

	{//Boss stuff
		MeshBuilder::GetInstance()->GenerateSprite("bossroomobjects", 6, 2, 2.f);
		MeshBuilder::GetInstance()->GetMesh("bossroomobjects")->textureID = LoadTGA("Image//bossroomobjects_spritesheet.tga");
		AnimationManager::GetInstance("bossroomobjects")->AddAnimation("grave_range", new Animation(1, 1));
		AnimationManager::GetInstance("bossroomobjects")->AddAnimation("grave_melee", new Animation(3, 3));
		AnimationManager::GetInstance("bossroomobjects")->AddAnimation("grave_mix", new Animation(5, 5));

		MeshBuilder::GetInstance()->GenerateCircle("grave_range_refresh_effect", Color(0.95f, 0.32f, 0.39f));
		MeshBuilder::GetInstance()->GenerateCircle("grave_melee_refresh_effect", Color(0.24f, 0.58f, 0.875f));

		MeshBuilder::GetInstance()->GenerateSprite("blubby", 4, 9, 2.f);
		MeshBuilder::GetInstance()->GetMesh("blubby")->textureID = LoadTGA("Image//blubby.tga");
		AnimationManager::GetInstance("blubby")->AddAnimation("idle", new Animation(18, 19));
		MeshBuilder::GetInstance()->GenerateSprite("blubby_ripple", 4, 9, 2.f);
		MeshBuilder::GetInstance()->GetMesh("blubby_ripple")->textureID = LoadTGA("Image//blubby_ripple.tga");
		AnimationManager::GetInstance("blubby_ripple")->AddAnimation("idle", new Animation(18, 19));

		float attackDuration = 1.0f;
		AnimationManager::GetInstance("blubby")->AddAnimation("charging", new Animation(27, 31, attackDuration, false));
		AnimationManager::GetInstance("blubby")->AddAnimation("stop", new Animation(34, 35));

		float bossIdleAnimSpeed = 1.0f;
		AnimationManager::GetInstance("blubby")->AddAnimation("phase1", new Animation(0, 6, bossIdleAnimSpeed));
		
		float chargeTime = 2.f;
		MeshBuilder::GetInstance()->GenerateSprite("boss_lazer", 1, 12, 1.f);
		MeshBuilder::GetInstance()->GetMesh("boss_lazer")->textureID = LoadTGA("Image//spritesheet_angryblubby_laser.tga");
		AnimationManager::GetInstance("boss_lazer")->AddAnimation("charging", new Animation(0, 7, chargeTime, false));
		AnimationManager::GetInstance("boss_lazer")->AddAnimation("attack", new Animation(10, 10));

		float bossMinionAnimSpeed = 0.5f;
		MeshBuilder::GetInstance()->GenerateSprite("boss_minion", 1, 2, 2.f);
		MeshBuilder::GetInstance()->GetMesh("boss_minion")->textureID = LoadTGA("Image//boss_minion_spritesheet.tga");
		MeshBuilder::GetInstance()->GenerateSprite("boss_minion_ripple", 1, 2, 2.f);
		MeshBuilder::GetInstance()->GetMesh("boss_minion_ripple")->textureID = LoadTGA("Image//spritesheet_blobling_ripple.tga");
		AnimationManager::GetInstance("boss_minion")->AddAnimation("movement", new Animation(0, 1, bossMinionAnimSpeed));

        bossEntity = Create::skywhaleEntity(entityManager, "blubby", "phase1", Vector3(60, 70, 0), Vector3(20, 20, 20), Vector3(0, 0, 0), Vector3(worldWidth, worldHeight* 0.9, 0), 100.f, 15.f, 75.f, 5.f);

        { // Boss Healthbar Init
            MeshBuilder::GetInstance()->GenerateQuad("boss_healthbar_front", Color(1.f, 0.0f, 0.0f), 1.f);
            MeshBuilder::GetInstance()->GetMesh("boss_healthbar_front")->textureID = LoadTGA("Image//boss_healthbar_front.tga");

			MeshBuilder::GetInstance()->GenerateQuad("boss_healthbar_front_2", Color(1.f, 0.0f, 0.0f), 1.f);
			MeshBuilder::GetInstance()->GetMesh("boss_healthbar_front_2")->textureID = LoadTGA("Image//boss_healthbar_front_2.tga");

            MeshBuilder::GetInstance()->GenerateQuad("boss_healthbar_back", Color(1.f, 0.0f, 0.0f), 1.f);
            MeshBuilder::GetInstance()->GetMesh("boss_healthbar_back")->textureID = LoadTGA("Image//boss_healthbar_back.tga");

            healthBar = Create::HealthBarRendererEntity(entityManager, "boss_healthbar_back", "boss_healthbar_front", Vector3(canvasWidth * 0.7f, canvasHeight * 0.1f, 0), Vector3(canvasWidth * 0.4f, canvasHeight * 0.07f, 1), bossEntity->GetMaxHealth());
        }
    }
    
    {// Rock Aesthetics
        AnimationManager::GetInstance("bossroomobjects")->AddAnimation("bossroomobject1", new Animation(6, 6));
        AnimationManager::GetInstance("bossroomobjects")->AddAnimation("bossroomobject2", new Animation(8, 8));
        AnimationManager::GetInstance("bossroomobjects")->AddAnimation("bossroomobject3", new Animation(9, 9));

        for (int i = 0; i < 25; ++i)
        {
            Vector3 position(Math::RandFloatMinMax(worldWidth * 0.1, worldWidth * 0.9), Math::RandFloatMinMax(worldHeight * 0.1, worldHeight * 0.9), -0.9f);
            
            int randImage = rand() % 3;
            std::string imageName;
            switch (randImage)
            {
            case 0:
                imageName = "bossroomobject1";
                break;
            case 1:
                imageName = "bossroomobject2";
                break;
            case 2:
                imageName = "bossroomobject3";
                break;
            default:
                imageName = "quad";
                break;
            }

            Create::environmentObject(entityManager, "bossroomobjects", imageName, position, Vector3(3, 3, 3));
        }
    }
    
	
	{//Create reflections
		for (auto q : entityManager->GetList())
		{
			PlayerRangeEntity* rangePlayer = dynamic_cast<PlayerRangeEntity*>(q);
			PlayerMeleeEntity* meleePlayer = dynamic_cast<PlayerMeleeEntity*>(q);
			SkyWhale* skyWhale = dynamic_cast<SkyWhale*>(q);
			BossMinion* bossMinion = dynamic_cast<BossMinion*>(q);
	
			if (rangePlayer)
			{
				reflectionMap[q] = Create::Sprite3DObject(entityManager, "player_staff_ripple", Vector3(), q->GetScale());
			}
			else if (meleePlayer)
			{
				reflectionMap[q] = Create::Sprite3DObject(entityManager, "player_shield_ripple", Vector3(), q->GetScale());
			}
			else if (skyWhale)
			{
				reflectionMap[q] = Create::Sprite3DObject(entityManager, "blubby_ripple", Vector3(), q->GetScale());
			}
			else if (bossMinion)
			{
				reflectionMap[q] = Create::Sprite3DObject(entityManager, "boss_minion_ripple", Vector3(), q->GetScale());
			}
		}
	}

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glClearColor(0.14f, 0.14f, 0.24f, 1);
	SoundManager::GetInstance()->PlayBGM("bossfight");
}

void SceneBoss::UpdateInputs(double dt)
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

    {// Cheat Code
        if (GameInfo::GetInstance()->GetHealth() <= 0)
        {
            SceneManager::GetInstance()->SetActiveScene("MenuState");
        }
    
        if (KeyboardController::GetInstance()->IsKeyPressed('H'))
        {
            GameInfo::GetInstance()->TakeDamage(-1);
        }

        if (KeyboardController::GetInstance()->IsKeyPressed('C'))
        {
            if (bossEntity)
            {
                bossEntity->SetSwitchPhase();
            }
        }
    }
}

void SceneBoss::Update(double dt)
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
    {//Player camera bounds
        ScrollingCamera* cam = dynamic_cast<ScrollingCamera*>(camera);
        if (cam)
        {
            Vector3 halfOrtho = Vector3(orthoWidth, orthoHeight, -2) * 0.5f;//offset z pos cause of camera position
            for (auto q : GameInfo::GetInstance()->GetPlayersList())
                q->GetCharacter()->HandleOutOfBounds(camera->GetPosition() - halfOrtho +  Vector3(0, +orthoHeight * 0.1f), camera->GetPosition() + halfOrtho + Vector3(0, -orthoHeight * 0.1f), dt);
        }
    }
	{//Update reflections
		{//Create reflections for new objects
			std::map<EntityBase*, SpriteEntity*>::iterator it;

			for (auto q : entityManager->GetList())
			{
				it = reflectionMap.find(q);
				if (it != reflectionMap.end())
					continue;
				else
				{//Create reflection for new entities
					BossMinion* bossMinion = dynamic_cast<BossMinion*>(q);

					if (bossMinion)
					{
						reflectionMap[q] = Create::Sprite3DObject(entityManager, "boss_minion_ripple", Vector3(), q->GetScale());
					}
				}
			}
		}
		{//Update reflectionMap
			std::map<EntityBase*, SpriteEntity*>::iterator it;
			for (it = reflectionMap.begin(); it != reflectionMap.end();)
			{
				if ((*it).first->GetIsDead() || (*it).first->GetIsDone())
				{
					it = reflectionMap.erase(it);
					continue;
				}

				Vector3 pos = (*it).first->GetPosition();
				pos.y -= ((*it).first->GetScale().y * 2.0f + 0.018f);
				pos.z = -2.0f;

				PlayerEntityBase* player = dynamic_cast<PlayerEntityBase*>((*it).first);
				EnemyBase* enemyBase = dynamic_cast<EnemyBase*>((*it).first);

				if (player)
				{
					(*it).second->animationPlayer.m_currentFrame = player->animationPlayer.m_currentFrame;
				}
				else if (enemyBase)
				{
					(*it).second->animationPlayer.m_currentFrame = enemyBase->GetAnimationPlayer()->m_currentFrame;
				}

				(*it).second->SetPosition(pos);
				++it;
			}
		}
	}

    {// Update Healthbar
        if (bossEntity)
        {
            if (!bossEntity->isAngry)
            {
                SpawnState* spawnState = dynamic_cast<SpawnState*>(bossEntity->GetCurrentStateManager()->GetCurrentState());

                if (spawnState)
                {
                    currentBossHeath = Math::Lerp(currentBossHeath, (float)spawnState->GetNumberOfGravesLeft(), dt * 1.5);
                    healthBar->SetCurrentHealth(currentBossHeath);
                    healthBar->SetMaxHealth(spawnState->GetMaxNumberOfGraves());
                }
                else
                {
                    healthBar->SetCurrentHealth(bossEntity->GetHealth());
                    healthBar->SetMaxHealth(bossEntity->GetMaxHealth());
                }
            }
            else
            {
                currentBossHeath = Math::Lerp(currentBossHeath, (float)bossEntity->GetHealth(), dt * 1.5);
                healthBar->SetCurrentHealth(currentBossHeath);
                healthBar->SetMaxHealth(bossEntity->GetMaxHealth());

				healthBar->SetFrontMesh(MeshBuilder::GetInstance()->GetMesh("boss_healthbar_front_2"));
            }


            if (bossEntity->GetIsDead() || bossEntity->GetIsDead())
                bossEntity = nullptr;
        }
        else
        {
            healthBar->SetShouldRender(false);
        }
    }
}

void SceneBoss::Render()
{
	SceneBase2D::Render();
}

void SceneBoss::Exit()
{
	SceneBase2D::Exit();

	entityManager->ClearList();
}