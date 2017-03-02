#include "IntroState.h"
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
#include "Animation.h"

IntroState::IntroState()
{
	entityManager = new EntityManager();
}

IntroState::~IntroState()
{
	if (entityManager)
	{
		entityManager->ClearList();
		delete entityManager;
		entityManager = NULL;
	}
}

void IntroState::Init()
{
	SceneBase2D::Init();


	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	Application::SetCursorMode2D();

    MeshBuilder::GetInstance()->GenerateText("customFont", 16, 16);
    MeshBuilder::GetInstance()->GetMesh("customFont")->textureID = LoadTGA("Image//game_font.tga");;

	MeshBuilder::GetInstance()->GenerateSprite("player_shield", 16, 4, 2.0f);
	MeshBuilder::GetInstance()->GetMesh("player_shield")->textureID = LoadTGA("Image//spritesheet_shield.tga");
	MeshBuilder::GetInstance()->GenerateSprite("player_shield_melee", 8, 6, 2.0f);
	MeshBuilder::GetInstance()->GetMesh("player_shield_melee")->textureID = LoadTGA("Image//spritesheet_shield_melee.tga");
	MeshBuilder::GetInstance()->GenerateQuad("player_shield_guard", Color(), 2.0f);
	MeshBuilder::GetInstance()->GetMesh("player_shield_guard")->textureID = LoadTGA("Image//spritesheet_shield_guard.tga");
	MeshBuilder::GetInstance()->GenerateSprite("player_shield_ripple", 16, 4, 2.0f);
	MeshBuilder::GetInstance()->GetMesh("player_shield_ripple")->textureID = LoadTGA("Image//spritesheet_shield_ripple.tga");
	MeshBuilder::GetInstance()->GenerateQuad("player_shield_hit_effect", Color(0.4f, 0.4f, 0.7f));
	//MeshBuilder::GetInstance()->GetMesh("player_shield_hit_effect")->textureID = LoadTGA("Image//spritesheet_staff_projectile.tga");
	MeshBuilder::GetInstance()->GenerateQuad("player_shield_block_effect", Color(0, 0, 0.7f));
	MeshBuilder::GetInstance()->GetMesh("player_shield_block_effect")->textureID = LoadTGA("Image//sprite_impact.tga");
	
	MeshBuilder::GetInstance()->GenerateSprite("player_staff", 16, 4, 2.0f);
	MeshBuilder::GetInstance()->GetMesh("player_staff")->textureID = LoadTGA("Image//spritesheet_staff.tga");
	MeshBuilder::GetInstance()->GenerateSprite("player_staff_ripple", 16, 4, 2.0f);
	MeshBuilder::GetInstance()->GetMesh("player_staff_ripple")->textureID = LoadTGA("Image//spritesheet_staff_ripple.tga");
	MeshBuilder::GetInstance()->GenerateQuad("player_staff_projectile", Color(), 2.0f);
	MeshBuilder::GetInstance()->GetMesh("player_staff_projectile")->textureID = LoadTGA("Image//spritesheet_staff_projectile.tga");
	MeshBuilder::GetInstance()->GenerateSprite("player_staff_hit_effect", 1, 5);
	MeshBuilder::GetInstance()->GetMesh("player_staff_hit_effect")->textureID = LoadTGA("Image//sprite_explosion.tga");
	AnimationManager::GetInstance("player_staff_hit_effect")->AddAnimation("default", new Animation(0, 4, 0.2f));

	float animSpeed = 0.6f;
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

    AnimationManager::GetInstance("player_staff")->AddAnimation("left", new Animation(32, 35, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("right", new Animation(36, 39, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("up_right", new Animation(44, 47, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("up_left", new Animation(40, 43, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("up", new Animation(48, 51, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("down", new Animation(52, 55, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("down_left", new Animation(56, 59, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("down_right", new Animation(60, 63, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("staff_left", new Animation(4, 7, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("staff_right", new Animation(0, 3, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("staff_up_right", new Animation(12, 15, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("staff_up_left", new Animation(8, 11, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("staff_up", new Animation(16, 19, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("staff_down", new Animation(20, 23, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("staff_down_left", new Animation(24, 27, animSpeed));
	AnimationManager::GetInstance("player_staff")->AddAnimation("staff_down_right", new Animation(28, 31, animSpeed));

	animSpeed = 0.3f;
	AnimationManager::GetInstance("player_shield_melee")->AddAnimation("right", new Animation(0, 5, animSpeed));
	AnimationManager::GetInstance("player_shield_melee")->AddAnimation("left", new Animation(6, 11, animSpeed));
	AnimationManager::GetInstance("player_shield_melee")->AddAnimation("up_right", new Animation(12, 17, animSpeed));
	AnimationManager::GetInstance("player_shield_melee")->AddAnimation("up_left", new Animation(18, 23, animSpeed));
	AnimationManager::GetInstance("player_shield_melee")->AddAnimation("up", new Animation(24, 29, animSpeed));
	AnimationManager::GetInstance("player_shield_melee")->AddAnimation("down", new Animation(30, 35, animSpeed));
	AnimationManager::GetInstance("player_shield_melee")->AddAnimation("down_left", new Animation(36, 41, animSpeed));
	AnimationManager::GetInstance("player_shield_melee")->AddAnimation("down_right", new Animation(42, 47, animSpeed));

	MeshBuilder::GetInstance()->GenerateCircle("shadow", Color(0, 0, 0), 18.0f);
	MeshBuilder::GetInstance()->GetMesh("shadow")->textureID = LoadTGA("Image//shadow.tga");

	MeshBuilder::GetInstance()->GenerateQuad("shield", Color(1.0f, 1.0f, 0.0f));
	//MeshBuilder::GetInstance()->GetMesh("shield")->textureID = LoadTGA("Image//playBtn.tga");
	MeshBuilder::GetInstance()->GenerateSprite("projectile_enemy", 1, 2, 2.0f);
	MeshBuilder::GetInstance()->GetMesh("projectile_enemy")->textureID = LoadTGA("Image//spritesheet_enemybullet.tga");
	AnimationManager::GetInstance("projectile_enemy")->AddAnimation("default", new Animation(0, 1, 0.2f));
	MeshBuilder::GetInstance()->GenerateQuad("enemy_hit_effect", Color(0.5f, 0.0f, 0.0f));
	//MeshBuilder::GetInstance()->GetMesh("enemy_hit_effect")->textureID = LoadTGA("Image//spritesheet_staff_projectile.tga");

	MeshBuilder::GetInstance()->GenerateCircle("circle", Color(1, 0, 1), 36, 1);
    MeshBuilder::GetInstance()->GetMesh("circle")->textureID = LoadTGA("Image//transTexture.tga");

	MeshBuilder::GetInstance()->GenerateQuad("playBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("playBtn")->textureID = LoadTGA("Image//playBtn.tga");
	
	MeshBuilder::GetInstance()->GenerateQuad("page1", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("page1")->textureID = LoadTGA("Image//GJ_cutscene1.tga");
	MeshBuilder::GetInstance()->GenerateQuad("page2", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("page2")->textureID = LoadTGA("Image//GJ_cutscene2.tga");
	MeshBuilder::GetInstance()->GenerateQuad("page3", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("page3")->textureID = LoadTGA("Image//GJ_cutscene3.tga");
	MeshBuilder::GetInstance()->GenerateQuad("page4", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("page4")->textureID = LoadTGA("Image//GJ_cutscene4.tga");

	pages[0].mesh = MeshBuilder::GetInstance()->GetMesh("page1");
	pages[1].mesh = MeshBuilder::GetInstance()->GetMesh("page2");
	pages[2].mesh = MeshBuilder::GetInstance()->GetMesh("page3");
	pages[3].mesh = MeshBuilder::GetInstance()->GetMesh("page4");

	for (int i = 0; i < NUM_PAGES; ++i)
	{
		pages[i].flipping = false;
		pages[i].flipValue = 0.0f;
		pages[i].flipDirection = 1;
	}

	orthoHeight = (float)Application::GetInstance().GetWindowHeight();
	orthoWidth = (float)Application::GetInstance().GetWindowWidth();
	worldHeight = orthoHeight;
	worldWidth = orthoHeight;
	canvasWidth = orthoWidth;
	canvasHeight = orthoHeight;

	float halfWindowWidth = orthoWidth * 0.5f;
	float halfWindowHeight = orthoHeight * 0.5f;

	camera->Init(Vector3(halfWindowWidth, halfWindowHeight, 1), Vector3(halfWindowWidth, halfWindowHeight, 0), Vector3(0, 1, 0));

	skipBtn = Create::button(entityManager, "playBtn", Vector3(canvasWidth - 50, canvasHeight - 15, 0.0f), Vector3(100, 30, 1));
	skipBtn->SetShouldRender(false);
	nextPageBtn = Create::button(entityManager, "playBtn", Vector3(canvasWidth - 20, canvasHeight * 0.5f, 0.0f), Vector3(40, 40, 1));
	nextPageBtn->SetShouldRender(false);
	prevPageBtn = Create::button(entityManager, "playBtn", Vector3(20, canvasHeight * 0.5f, 0.0f), Vector3(40, 40, 1));
	prevPageBtn->SetShouldRender(false);
}

void IntroState::UpdateInputs(double dt)
{
	float mouseX, mouseY;
	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
	mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
	mouseX *= canvasWidth / Application::GetInstance().GetWindowWidth();
	mouseY *= canvasHeight / Application::GetInstance().GetWindowHeight();
	Vector3 mousePos(mouseX, mouseY, 0);

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		if (skipBtn->CheckCollision(mousePos))
		{
			SceneManager::GetInstance()->SetActiveScene("MenuState");
		}
		else if (nextPageBtn->CheckCollision(mousePos))
		{
			NextPage();
		}
		else if (prevPageBtn->CheckCollision(mousePos))
		{
			PrevPage();
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RIGHT))
	{
		NextPage();
	}
	else if (KeyboardController::GetInstance()->IsKeyPressed(VK_LEFT))
	{
		PrevPage();
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
	{
		SceneManager::GetInstance()->SetActiveScene("MenuState");
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE))
	{
		Application::Close();
	}
}

void IntroState::Update(double dt)
{
	for (int i = 0; i < NUM_PAGES; ++i)
	{
		if (pages[i].flipping)
		{
			pages[i].flipValue += pages[i].flipDirection * PAGE_FLIP_SPEED * dt;
			if (pages[i].flipDirection > 0 && pages[i].flipValue > 90.0f)
			{
				pages[i].flipValue = 90.0f;
				pages[i].flipping = false;
			}
			else if (pages[i].flipDirection < 0 && pages[i].flipValue < 0.0f)
			{
				pages[i].flipValue = 0.0f;
				pages[i].flipping = false;
			}
		}
	}
}

void IntroState::Render()
{
	float halfOrthoWidth = orthoWidth * 0.5f;
	float halfOrthoHeight = orthoHeight * 0.5f;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfOrthoWidth, halfOrthoWidth, -halfOrthoHeight, halfOrthoHeight, -10, 10);
	GraphicsManager::GetInstance()->AttachCamera(camera);
	entityManager->Render();

	{//render pages
		float halfWindowWidth = orthoWidth * 0.5f;
		float halfWindowHeight = orthoHeight * 0.5f;
		MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

		for (int i = 0; i < NUM_PAGES; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(halfWindowWidth, halfWindowHeight, -1.0f - (i * 0.1f));
			modelStack.Scale(orthoWidth, orthoHeight, 1);
			modelStack.Translate(-0.5f, 0, 0);
			modelStack.Rotate(-pages[i].flipValue, 0, 1, 0);
			modelStack.Translate(0.5f, 0, 0);
			RenderHelper::RenderMesh(pages[i].mesh);
			modelStack.PopMatrix();
		}
	}
	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(
		0, canvasWidth,		// minX - maxX
		0, canvasHeight,	// minY - maxY
		-10, 10);											// minZ - maxZ
	GraphicsManager::GetInstance()->DetachCamera();
	entityManager->RenderUI();
}

void IntroState::NextPage()
{
	for (int i = 0; i < NUM_PAGES - 1; ++i)
	{
		if (pages[i].flipValue >= 90.0f || pages[i].flipping)
			continue;
		else
		{
			pages[i].flipping = true;
			pages[i].flipDirection = 1;
			break;
		}
	}
}
void IntroState::PrevPage()
{
	for (int i = NUM_PAGES - 2; i >= 0; --i)
	{
		if (pages[i].flipValue > 0.0f && pages[i].flipDirection > 0)
		{
			pages[i].flipping = true;
			pages[i].flipDirection = -1;

			break;
		}
	}
}

void IntroState::Exit()
{
	SceneBase2D::Exit();

	MeshBuilder::GetInstance()->RemoveMesh("playBtn");

	if (entityManager)
	{
		entityManager->ClearList();
	}
}