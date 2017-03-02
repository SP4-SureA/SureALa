#include "SceneEndless.h"
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

#include "..\PlayerInfo\playerRangeEntity.h"
#include "../PlayerInfo/PlayerMeleeEntity.h"
#include "..\ScrollingEntity.h"
#include "..\CloudEntity\CloudFactory.h"
#include "..\FloorTiles\FloorTile.h"
#include "..\FloorTiles\FloorTilesManager.h"
#include "../Boss/Phase1/GraveStoneEntity.h"
#include "../Boss/SkyWhale.h"
#include "..\Particle\ParticleScale.h"
#include "../Enemy\Flappy.h"
#include "..\Enemy\Floopy.h"
#include "../Enemy\Bitey.h"
#include "..\SpriteEntity.h"
#include "../SP4/Wall.h"

#include "HealthUI.h"
#include "HealthBarRenderer.h"

SceneEndless::SceneEndless() :
currentLevel(1),
stageDelay(0)
{
	this->entityManager = EntityManager::GetInstance();
	for (int i = 0; i < 2; ++i)
		cloudScrollUI[i] = NULL;
}

SceneEndless::~SceneEndless()
{
}

void SceneEndless::Init()
{
	SceneBase2D::Init();

	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("cloud_1_translucent", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cloud_1_translucent")->textureID = LoadTGA("Image//cloud_1_translucent.tga");
	MeshBuilder::GetInstance()->GenerateQuad("cloud_3_translucent", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cloud_3_translucent")->textureID = LoadTGA("Image//cloud_3_translucent.tga");
	MeshBuilder::GetInstance()->GenerateQuad("cloud_4_translucent", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cloud_4_translucent")->textureID = LoadTGA("Image//cloud_4_translucent.tga");

	MeshBuilder::GetInstance()->GenerateQuad("cloud_1_opaque", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cloud_1_opaque")->textureID = LoadTGA("Image//cloud_1_opaque.tga");
	MeshBuilder::GetInstance()->GenerateQuad("cloud_3_opaque", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cloud_3_opaque")->textureID = LoadTGA("Image//cloud_3_opaque.tga");
	MeshBuilder::GetInstance()->GenerateQuad("cloud_4_opaque", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("cloud_4_opaque")->textureID = LoadTGA("Image//cloud_4_opaque.tga");

	MeshBuilder::GetInstance()->GenerateQuad("bg_sky", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("bg_sky")->textureID = LoadTGA("Image//scrolling_bg2.tga");
	MeshBuilder::GetInstance()->GenerateQuad("background", Color(1, 1, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("background")->textureID = LoadTGA("Image//sprite_background.tga");
	MeshBuilder::GetInstance()->GenerateQuad("bossdoor", Color(0.7f, 0.2f, 0.2f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("bossdoor")->textureID = LoadTGA("Image//bossdoor.tga");

	MeshBuilder::GetInstance()->GenerateSprite("flappy", 1, 5, 2.f);
	MeshBuilder::GetInstance()->GetMesh("flappy")->textureID = LoadTGA("Image//flappy.tga");
	AnimationManager::GetInstance("flappy")->AddAnimation("move", new Animation(0, 3));
	AnimationManager::GetInstance("flappy")->AddAnimation("attack", new Animation(4, 4));

	MeshBuilder::GetInstance()->GenerateSprite("floopy", 1, 4, 2.f);
	MeshBuilder::GetInstance()->GetMesh("floopy")->textureID = LoadTGA("Image//floopy.tga");
	AnimationManager::GetInstance("floopy")->AddAnimation("move", new Animation(0, 3));

	MeshBuilder::GetInstance()->GenerateSprite("bitey", 1, 6, 2.f);
	MeshBuilder::GetInstance()->GetMesh("bitey")->textureID = LoadTGA("Image//bitey.tga");
	AnimationManager::GetInstance("bitey")->AddAnimation("move", new Animation(0, 3));
	AnimationManager::GetInstance("bitey")->AddAnimation("attack_downwards", new Animation(1, 1));
	AnimationManager::GetInstance("bitey")->AddAnimation("attack_right", new Animation(4, 4));
	AnimationManager::GetInstance("bitey")->AddAnimation("attack_left", new Animation(5, 5));

	MeshBuilder::GetInstance()->GenerateQuad("flapflap", Color(0.7f, 0.7f, 0.7f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("flapflap")->textureID = LoadTGA("Image//flapflap.tga");
	MeshBuilder::GetInstance()->GenerateQuad("quad2", Color(0.9f, 0.7f, 0.7f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("quad2")->textureID = LoadTGA("Image//transTexture.tga");

	MeshBuilder::GetInstance()->GenerateQuad("heart", Color(0.7f, 0.7f, 0.7f), 1.f);
	MeshBuilder::GetInstance()->GetMesh("heart")->textureID = LoadTGA("Image//heart.tga");
	
	orthoWidth = 100;
	orthoHeight = orthoWidth * 1080 / 1920;// *(Application::GetInstance().GetWindowHeight() / (float)Application::GetInstance().GetWindowWidth());

	worldWidth = 100.0f;
	worldHeight = 100.0f;
	float halfWorldWidth = worldWidth * 0.5f;
	float halfWorldHeight = worldHeight * 0.5f;
	pauseGame = false;
	
	camera->Init(Vector3(halfWorldWidth, orthoHeight * 0.5f, 1), Vector3(halfWorldWidth, orthoHeight * 0.5f, 0), Vector3(0, 1, 0));
	
	{
		scrollSpeed = 20.0f;
		canvasWidth = worldWidth;
		canvasHeight = worldHeight;
		float halfCanvasWidth = canvasWidth * 0.5f;
		float halfCanvasHeighth = canvasHeight * 0.5f;

		cloudScrollUI[0] = Create::scrollingEntity(entityManager, "bg_sky",
			Vector3(halfCanvasWidth, halfCanvasHeighth, -2),	//position
			Vector3(0, -1, 0), scrollSpeed,						//scroll direction, scrollSpeed
			Vector3(-halfCanvasWidth, -halfCanvasHeighth, 0),	//min boundary
			Vector3(halfCanvasWidth, halfCanvasHeighth, 0),		//max boundary
			Vector3(canvasWidth, canvasHeight, 1)				//scale
			);
		cloudScrollUI[1] = Create::scrollingEntity(entityManager, "bg_sky",
			Vector3(halfCanvasWidth, halfCanvasHeighth, -2),	//position
			Vector3(0, -1, 0), scrollSpeed,						//scroll direction, scrollSpeed
			Vector3(halfCanvasWidth, halfCanvasHeighth, 0),		//min boundary
			Vector3(canvasWidth*1.5f, canvasHeight*1.5f, 0),	//max boundary
			Vector3(canvasWidth, canvasHeight, 1)				//scale
			);
		Create::scrollingEntity(entityManager, "background",
			Vector3(halfCanvasWidth, halfCanvasHeighth, -3),	//position
			Vector3(0, -1, 0), scrollSpeed,						//scroll direction, scrollSpeed
			Vector3(-halfCanvasWidth, -halfCanvasHeighth, 0),	//min boundary
			Vector3(halfCanvasWidth, halfCanvasHeighth, 0),		//max boundary
			Vector3(canvasWidth, canvasHeight, 1)				//scale
			);
		Create::scrollingEntity(entityManager, "background",
			Vector3(halfCanvasWidth, halfCanvasHeighth, -3),	//position
			Vector3(0, -1, 0), scrollSpeed,						//scroll direction, scrollSpeed
			Vector3(halfCanvasWidth, halfCanvasHeighth, 0),		//min boundary
			Vector3(canvasWidth*1.5f, canvasHeight*1.5f, 0),	//max boundary
			Vector3(canvasWidth, canvasHeight, 1)				//scale
			);
	}

	Create::healthUIEntity(entityManager, "heart", Vector3((canvasHeight * 0.1f) * 1080 / 1920, canvasHeight * 0.1f, 1), Vector3(canvasWidth, canvasHeight, 0));

	flapflapOffsetLimit = 15.0f;
	flapflapOffsetSpeed = 5.0f;
	flapflapXoffset = 0;
	flapflapDirection = 1;

	groundEntity = Create::Entity(entityManager, "flapflap", Vector3(halfWorldWidth, 15.0f, -1), Vector3(60.0f, 60.0f, 0));
	{//for platform
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth + 0.5f, 24.0f, -0.9f), Vector3(20.0f, 25.0f, 0)));
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth + 0.5f, 24.0f, -0.9f), Vector3(15.0f, 30.0f, 0)));
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth + 0.5f, 20.0f, -0.9f), Vector3(32.0f, 10.0f, 0)));
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth + 0.5f, 25.5f, -0.9f), Vector3(40.0f, 10.0f, 0)));
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth + 0.5f, 24.0f, -0.9f), Vector3(48.5f, 5.0f, 0)));
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth + 0.5f, 23.5f, -0.9f), Vector3(23.0f, 23.0f, 0)));
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth + 5.0f, 40.4f, -0.9f), Vector3(3.0f, 10.0f, 0)));
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth - 4.0f, 40.4f, -0.9f), Vector3(3.0f, 10.0f, 0)));
		floorList.push_back(Create::floorTileEntity(entityManager, "quad2", Vector3(halfWorldWidth + 0.5f, 25.5f, -0.9f), Vector3(35.0f, 13.0f, 0)));
		
		Vector3 playerPos = floorList.back()->GetPosition();
		playerPos.z = 0.0f;
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			q->GetCharacter()->SetPosition(playerPos);
			q->GetCharacter()->SetSpawnPoint(playerPos);
			entityManager->GetInstance()->RemoveEntity(q->GetCharacter());
			entityManager->AddEntity(q->GetCharacter());
		}
		for (auto q : floorList)
		{
			q->SetShouldRender(false);
		}
	}

	yDestination = groundEntity->GetPosition().y + groundEntity->GetScale().y * 0.5f;

    cloudFactory = Create::cloudFactoryEntity(entityManager, 25.0f, 1.5f, Vector3(worldWidth*0.2f, 0.0f, 0.0f), Vector3(worldWidth*0.8f, worldHeight, 0.0f));

    WavesInit();

    { // Progress Init
        MeshBuilder::GetInstance()->GenerateQuad("progress_bar", Color(1.f, 0.0f, 0.0f), 1.f);
        MeshBuilder::GetInstance()->GetMesh("progress_bar")->textureID = LoadTGA("Image//progress_bar.tga");

        MeshBuilder::GetInstance()->GenerateQuad("boss_healthbar_back", Color(1.f, 0.0f, 0.0f), 1.f);
        MeshBuilder::GetInstance()->GetMesh("boss_healthbar_back")->textureID = LoadTGA("Image//boss_healthbar_back.tga");

        MeshBuilder::GetInstance()->GenerateQuad("progress_flag", Color(1.f, 0.0f, 0.0f), 1.f);
        MeshBuilder::GetInstance()->GetMesh("progress_flag")->textureID = LoadTGA("Image//progress_flag.tga");

        progressBar = Create::HealthBarRendererEntity(entityManager, "boss_healthbar_back", "progress_bar", Vector3(canvasWidth * 0.975f, canvasHeight * 0.85f, 0), Vector3(canvasWidth * 0.7f, canvasHeight * 0.025f, 1), numberOfWaves);
        progressBar->SetRotateAngle(90);

        Create::Sprite2DObject(entityManager, "progress_flag", Vector3(canvasWidth * 0.975f, canvasHeight * 0.865f, 0), Vector3(2.5f, 3.f, 3.f));
    }

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	glClearColor(0, 0.45f, 0.8f, 1);

}

void SceneEndless::WavesInit()
{
    stageDelay = 10;

	{//Clears wavelist
		waveList.clear();
	}
    { // Initializing Waves
        std::vector<EnemyBase*> wave1;
        wave1.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 1.0f, 2.0f, currentLevel, Vector3(20, 110), Vector3(3, 3, 1)));
        wave1.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 1.0f, 2.0f, currentLevel, Vector3(80, 110), Vector3(3, 3, 1)));
        wave1[0]->AddWaypoint(Vector3(40, 45, 0));
        wave1[1]->AddWaypoint(Vector3(60, 45, 0));

        std::vector<EnemyBase*> wave2;
        wave2.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 1.0f, 2.0f, currentLevel, Vector3(-20, 80), Vector3(3, 3, 1)));
        wave2.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 1.0f, 2.0f, currentLevel, Vector3(150, 80), Vector3(3, 3, 1)));
        wave2[0]->AddWaypoint(Vector3(20, 45, 0));
        wave2[0]->AddWaypoint(Vector3(80, 45, 0));
        wave2[1]->AddWaypoint(Vector3(80, 37, 0));
        wave2[1]->AddWaypoint(Vector3(20, 37, 0));

        std::vector<EnemyBase*> wave3;
        Bitey* bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(20, 110), Vector3(3, 3, 1));
        bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
        wave3.push_back(bitey);
        bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(50, 110), Vector3(3, 3, 1));
        bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
        wave3.push_back(bitey);
        bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(80, 110), Vector3(3, 3, 1));
        bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
        wave3.push_back(bitey);
        wave3[0]->AddWaypoint(Vector3(45, 37, 0));
        wave3[1]->AddWaypoint(Vector3(50, 37, 0));
        wave3[2]->AddWaypoint(Vector3(55, 37, 0));
  
        std::vector<EnemyBase*> wave4;
        bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(20, 110), Vector3(3, 3, 1));
        bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
        wave4.push_back(bitey);
        bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(80, 110), Vector3(3, 3, 1));
        bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
        wave4.push_back(bitey);
        wave4[0]->AddWaypoint(Vector3(30, 37, 0));
        wave4[0]->AddWaypoint(Vector3(55, 37, 0));
        wave4[1]->AddWaypoint(Vector3(80, 37, 0));
        wave4[1]->AddWaypoint(Vector3(45, 37, 0));

        std::vector<EnemyBase*> wave5;
        { // Bities
            bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(20, 110), Vector3(3, 3, 1));
            bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
            wave5.push_back(bitey);
            bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(80, 110), Vector3(3, 3, 1));
            bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
            wave5.push_back(bitey);
            bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(20, 110), Vector3(3, 3, 1));
            bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
            wave5.push_back(bitey);
            bitey = Create::biteyAsset("bitey", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(80, 110), Vector3(3, 3, 1));
            bitey->SetBoundary(Vector3(0, 0, 0), Vector3(worldWidth, camera->GetPosition().y + orthoHeight * 0.5f, 0));
            wave5.push_back(bitey);
            wave5[0]->AddWaypoint(Vector3(15, 48, 0));
            wave5[0]->AddWaypoint(Vector3(45, 48, 0));
            wave5[1]->AddWaypoint(Vector3(25, 48, 0));
            wave5[1]->AddWaypoint(Vector3(55, 48, 0));
            wave5[2]->AddWaypoint(Vector3(35, 37, 0));
            wave5[2]->AddWaypoint(Vector3(65, 37, 0));
            wave5[3]->AddWaypoint(Vector3(45, 37, 0));
            wave5[3]->AddWaypoint(Vector3(75, 37, 0));
        }
        { // Floopys
            wave5.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(30, 110), Vector3(3, 3, 1)));
            wave5.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(40, 110), Vector3(3, 3, 1)));
            wave5.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(50, 110), Vector3(3, 3, 1)));
            wave5.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(60, 110), Vector3(3, 3, 1)));
            wave5.push_back(Create::floopyAsset("floopy", 200.0f + (currentLevel * 10), 20.0f + (currentLevel), 0.0f, 2.0f, currentLevel, Vector3(70, 110), Vector3(3, 3, 1)));
            wave5[4]->AddWaypoint(Vector3(25, 45, 0));
            wave5[4]->AddWaypoint(Vector3(35, 45, 0));
            wave5[5]->AddWaypoint(Vector3(35, 45, 0));
            wave5[5]->AddWaypoint(Vector3(45, 45, 0));
            wave5[6]->AddWaypoint(Vector3(45, 45, 0));
            wave5[6]->AddWaypoint(Vector3(55, 45, 0));
            wave5[7]->AddWaypoint(Vector3(55, 45, 0));
            wave5[7]->AddWaypoint(Vector3(65, 45, 0));
            wave5[8]->AddWaypoint(Vector3(65, 45, 0));
            wave5[8]->AddWaypoint(Vector3(75, 45, 0));
        }

        waveList.push_back(wave1);
        waveList.push_back(wave2);
        waveList.push_back(wave3);
        waveList.push_back(wave4);
        waveList.push_back(wave5);

        numberOfWaves = waveList.size();
    }

    waveCleared = false;
	currentProgress = 0;

    {//spawn first wave
        waveIndex = 0;
        for (auto q : waveList[waveIndex])
        {
            EntityManager::GetInstance()->AddEntity(q);
        }
    }
}

void SceneEndless::UpdateInputs(double dt)
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
		for (auto q : floorList)
		{
			q->SetShouldRender(!q->GetShouldRender());
		}
	}
	if (KeyboardController::GetInstance()->IsKeyDown('O'))
	{
		if (KeyboardController::GetInstance()->IsKeyPressed('P'))
		{
			for (auto q : waveList[waveIndex])
			{
				q->SetIsDead(true);
				Create::particleScale(EntityManager::GetInstance(), "heart", q->GetPosition(), Vector3(6, 6, 1), 0.2f, 0.1f, 0.2f);
			}
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		pauseGame = true;
		SceneManager::GetInstance()->OverlayScene("PauseState");
	}
}

void SceneEndless::Update(double dt)
{
	if (pauseGame)
		return;

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

	if (!waveCleared)//if wave not cleared
	{
		Vector3 camTarg = camera->GetPosition();
		camTarg.y = orthoHeight * 0.5f;
		camera->SetPosition(Vector3(camTarg.x, camTarg.y, 1));
		camera->SetTarget(Vector3(camTarg.x, camTarg.y, 0));

		{//Move manta
			flapflapXoffset += flapflapOffsetSpeed * flapflapDirection * dt;
			{//Move visual
				Vector3 pos = groundEntity->GetPosition();
				pos.x += flapflapOffsetSpeed * flapflapDirection * dt;
				groundEntity->SetPosition(pos);
			}
			{//Move floor tiles
				Vector3 pos;
				for (auto q : floorList)
				{
					pos = q->GetPosition();
					pos.x += flapflapOffsetSpeed * flapflapDirection * dt;
					q->SetPosition(pos);
				}
			}
			{//Move players
				Vector3 pos;
				for (auto q : GameInfo::GetInstance()->GetPlayersList())
				{
					pos = q->GetCharacter()->GetPosition();
					pos.x += flapflapOffsetSpeed * flapflapDirection * dt;
					q->GetCharacter()->SetPosition(pos);
				}
			}
			if (flapflapXoffset * flapflapDirection > flapflapOffsetLimit)
				flapflapDirection = -flapflapDirection;
		}
		
		if (waveList.size() > 0)//safety measure in case of an empty vector
		{
			//to count number of dead enemies
			std::vector<EnemyBase*>::iterator it;
			for (it = waveList[waveIndex].begin(); it != waveList[waveIndex].end();)
			{
				if ((*it)->GetIsDead() || (*it)->GetIsDone())
				{
					it = waveList[waveIndex].erase(it);
				}
				else
				{
					++it;
				}
			}
			if (waveList[waveIndex].size() <= 0)
			{
				//spawns next wave
				waveIndex = (waveIndex + 1) % waveList.size();//move index up. If reached limit, back to zero
				if (waveIndex <= 0)//if waveIndex reaches 0, it means end of wave
				{
					waveCleared = true;
				}
				else//spawns next wave
				{
					for (auto q : waveList[waveIndex])
					{
						EntityManager::GetInstance()->AddEntity(q);
					}
				}
			}
		}
		else
			waveCleared = true;
	}
    else
    {
        stageDelay -= dt;
        if (stageDelay <= 0)
        {
            waveCleared = false;
            ++currentLevel;
            WavesInit();
            GameInfo::GetInstance()->TakeDamage(-1);
        }
    }

    // Update Progress Bar
    {
        if (!waveCleared)
			currentProgress = Math::Lerp((double)currentProgress, (double)waveIndex, dt * 1.5);
        else
			currentProgress = Math::Lerp((double)currentProgress, (double)numberOfWaves, dt * 1.5);

		progressBar->SetCurrentHealth(currentProgress);
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
    }
}

void SceneEndless::Render()
{
	SceneBase2D::Render();
}

void SceneEndless::Exit()
{
	SceneBase2D::Exit();
	{//Clears wave
		waveList.clear();
		//std::vector<std::vector<EnemyBase*>>::iterator it;
		//while (it != waveList.end())
		//{ 
		//	(*it).clear();
		//	it = waveList.erase(it);
		//}
	}

	for (auto q : floorList)
	{
		FloorTilesManager::GetInstance()->RemoveFloor(q);
	}
	entityManager->ClearList();
}