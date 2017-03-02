#include "SelectionState.h"
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
#include "..\SP4\GameInfo.h"
#include "..\TextEntity.h"

#include "..\PlayerInfo\PlayerRangeEntity.h"
#include "..\PlayerInfo\PlayerMeleeEntity.h"

#include <sstream>

SelectionState::SelectionState():
gameStartTimer(0.0f)
{
	entityManager = new EntityManager();
	for (int i = 0; i < CHOICE_END; ++i)
	{
		charaPortrait[i].selectedMesh = NULL;
		charaPortrait[i].notSelectedMesh = NULL;
	}
	textEntity = NULL;
}

SelectionState::~SelectionState()
{
	if (entityManager)
	{
		entityManager->ClearList();
		delete entityManager;
		entityManager = NULL;
	}
}

void SelectionState::Init()
{
	SceneBase2D::Init();

	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	Application::SetCursorMode2D();

	MeshBuilder::GetInstance()->GenerateQuad("backBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("backBtn")->textureID = LoadTGA("Image//playBtn.tga");
	MeshBuilder::GetInstance()->GenerateQuad("selectionState_bg", Color(1.0f, 1.0f, 0.0f), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("selectionState_bg")->textureID = LoadTGA("Image//sprite_background.tga");

	charaPortrait[CHOICE_AVERRA].selectedMesh = MeshBuilder::GetInstance()->GenerateQuad("averra_selected", Color(1, 0, 0), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("averra_selected")->textureID = LoadTGA("Image//averra_selected.tga");
	charaPortrait[CHOICE_AVERRA].notSelectedMesh = MeshBuilder::GetInstance()->GenerateQuad("averra_notSelected", Color(1, 0, 0), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("averra_notSelected")->textureID = LoadTGA("Image//averra_portrait.tga");
	charaPortrait[CHOICE_ENNA].selectedMesh = MeshBuilder::GetInstance()->GenerateQuad("enna_selected", Color(0, 0, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("enna_selected")->textureID = LoadTGA("Image//enna_selected.tga");
	charaPortrait[CHOICE_ENNA].notSelectedMesh = MeshBuilder::GetInstance()->GenerateQuad("enna_notSelected", Color(0, 0, 1), 1.0f);
	MeshBuilder::GetInstance()->GetMesh("enna_notSelected")->textureID = LoadTGA("Image//enna_portrait.tga");

	canvasWidth = (float)Application::GetInstance().GetWindowWidth();
	canvasHeight = (float)Application::GetInstance().GetWindowHeight();
	orthoWidth = canvasWidth;
	orthoHeight = canvasHeight;
	worldWidth = orthoWidth;
	worldHeight = orthoHeight;

	float halfWorldWidth = orthoWidth * 0.5f;
	float halfWorldHeight = orthoHeight * 0.5f;

	camera->Init(Vector3(halfWorldWidth, halfWorldHeight, 1), Vector3(halfWorldWidth, halfWorldHeight, 0), Vector3(0, 1, 0));

	textEntity = Create::Text3DObject(entityManager, "customFont", Vector3(worldWidth * 0.05f, worldHeight * 0.1f, 0.0f), "Press Up to join, Special to ready", Vector3(50, 50, 1), Color());

	Create::Entity(entityManager, "selectionState_bg", Vector3(halfWorldWidth, halfWorldHeight, -1.0f), Vector3(orthoWidth, orthoHeight, 1));
	backBtn = Create::button(entityManager, "backBtn", Vector3(canvasWidth*0.1f, canvasHeight * 0.95f, 0.0f), Vector3(150, 45, 1));
	backBtn->SetShouldRender(false);

	GameInfo::GetInstance()->AddPlayer(new PlayerInfo());
	GameInfo::GetInstance()->AddPlayer(new PlayerInfo());
	GameInfo::GetInstance()->AddPlayer(new PlayerInfo());
	GameInfo::GetInstance()->GetPlayersList()[0]->LoadControls("Player1_Controls");
	GameInfo::GetInstance()->GetPlayersList()[1]->LoadControls("Player2_Controls");

	for (int i = 0; i < CHOICE_END; ++i)
	{
		charaPortrait[i].selected = false;
	}
	selectionInfoMap.clear();
	for (auto q : GameInfo::GetInstance()->GetPlayersList())
	{
		selectionInfoMap[q].portrait = Create::Entity(entityManager, "averra_notSelected", Vector3());

		selectionInfoMap[q].characterChoice = 0;
		selectionInfoMap[q].joinedGame = false;
		selectionInfoMap[q].ready = false;
		selectionInfoMap[q].portrait->SetShouldRender(false);
	}

	//GameInfo::GetInstance()->GetPlayersList().front()->SetCharacter(Create::playerRangeEntity(EntityManager::GetInstance(), "player_staff", 10.0f, 5.0f, 0.5f, 1.0f, Vector3(50.0f, 22.0f, 0), Vector3(3, 3, 1)));
	//GameInfo::GetInstance()->GetPlayersList().back()->SetCharacter(Create::playerMeleeEntity(EntityManager::GetInstance(), "player_shield", 10.0f, 5.0f, 0.5f, 1.0f, Vector3(50.0f, 22.0f, 0), Vector3(3, 3, 1)));
}

void SelectionState::UpdateInputs(double dt)
{
	SceneBase2D::UpdateInputs(dt);
	float mouseX, mouseY;
	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
	mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
	mouseX *= canvasWidth / Application::GetInstance().GetWindowWidth();
	mouseY *= canvasHeight / Application::GetInstance().GetWindowHeight();
	Vector3 mousePos(mouseX, mouseY, 0);

	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		if (backBtn->CheckCollision(mousePos))
		{
			SceneManager::GetInstance()->SetActiveScene("GameModeSelectionState");
		}
	}
	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE) || KeyboardController::GetInstance()->IsKeyPressed(VK_BACK))
	{
		SceneManager::GetInstance()->SetActiveScene("GameModeSelectionState");
	}
	for (auto q : GameInfo::GetInstance()->GetPlayersList())
	{
		if (KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_MOVEUP)) || 
			KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_MOVEDOWN)))
		{
			if (KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_MOVEUP)))
				selectionInfoMap[q].joinedGame = true;
			else if (!selectionInfoMap[q].ready)
				selectionInfoMap[q].joinedGame = false;
			
			selectionInfoMap[q].portrait->SetShouldRender(selectionInfoMap[q].joinedGame);

			int counter = 0;
			int numOfJoinedPlayers = 0;
			for (auto r : GameInfo::GetInstance()->GetPlayersList())
			{
				if (selectionInfoMap[r].joinedGame)
					numOfJoinedPlayers++;
			}
			float precentageWidth = 0.8f;
			float size = (canvasWidth * precentageWidth) / numOfJoinedPlayers;
			size = Math::Min(size, canvasHeight * 0.7f);
			for (auto r : GameInfo::GetInstance()->GetPlayersList())
			{
				if (selectionInfoMap[r].joinedGame == false)
					continue;
				if (numOfJoinedPlayers == 1)
					selectionInfoMap[r].portrait->SetPosition(Vector3(canvasWidth * 0.5f, canvasHeight * 0.5f, 0));
				else
					selectionInfoMap[r].portrait->SetPosition(Vector3(((canvasWidth * ((1.0f - precentageWidth) * 0.5)) + (size * 0.5f)) + (counter * size), canvasHeight * 0.5f, 0));
				selectionInfoMap[r].portrait->SetScale(Vector3(size, size, 1));

				counter++;
			}
		}
		else if (selectionInfoMap[q].joinedGame)
		{
			if (KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_SPECIAL)))
			{
				if (!charaPortrait[selectionInfoMap[q].characterChoice].selected || selectionInfoMap[q].ready)
				{
					selectionInfoMap[q].ready = !selectionInfoMap[q].ready;
					charaPortrait[selectionInfoMap[q].characterChoice].selected = selectionInfoMap[q].ready;
					if (selectionInfoMap[q].ready)
						selectionInfoMap[q].portrait->SetMesh(charaPortrait[selectionInfoMap[q].characterChoice].selectedMesh);
					else
						selectionInfoMap[q].portrait->SetMesh(charaPortrait[selectionInfoMap[q].characterChoice].notSelectedMesh);
				}
			}
			else if (!selectionInfoMap[q].ready)
			{
				if (KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_MOVELEFT)))
				{
					selectionInfoMap[q].characterChoice = abs((selectionInfoMap[q].characterChoice - 1) % CHOICE_END);
					selectionInfoMap[q].portrait->SetMesh(charaPortrait[selectionInfoMap[q].characterChoice].notSelectedMesh);
				}
				else if (KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_MOVERIGHT)))
				{
					selectionInfoMap[q].characterChoice = (selectionInfoMap[q].characterChoice + 1) % CHOICE_END;
					selectionInfoMap[q].portrait->SetMesh(charaPortrait[selectionInfoMap[q].characterChoice].notSelectedMesh);
				}
			}
		}
	}
}

void SelectionState::Update(double dt)
{
	int numberOfReadyPlayers = 0;
	int numberOfJoinedPlayers = 0;
	for (auto q : selectionInfoMap)
	{
		if (q.second.ready)
			numberOfReadyPlayers++;
		if (q.second.joinedGame)
			numberOfJoinedPlayers++;
	}
	if (numberOfJoinedPlayers > 1)//minumum 2 player game
	{
		if (numberOfReadyPlayers == numberOfJoinedPlayers)
		{
			if (gameStartTimer <= 0.0f)
			{
				StartGame();
			}
			else
			{
				gameStartTimer -= dt;
				std::stringstream ss;
				ss.precision(3);
				ss << "Game starting in: " << gameStartTimer;
				textEntity->SetText(ss.str());
			}
		}
		else
		{
			gameStartTimer = 1.0f;
			textEntity->SetText("Press Up to join, Special to ready");
		}
	}
}

void SelectionState::StartGame()
{
	int numOfJoinedPlayers = 0;
	for (auto q : GameInfo::GetInstance()->GetPlayersList())
	{
		if (selectionInfoMap[q].joinedGame)
			numOfJoinedPlayers++;
	}
	bool sameChoice = false;
	for (auto q : selectionInfoMap)
	{
		if (!q.second.joinedGame)
			continue;
		for (auto r : selectionInfoMap)
		{
			if (!r.second.joinedGame)
				continue;
			if (q.first == r.first)
				continue;

			if (q.second.characterChoice == r.second.characterChoice)
			{
				sameChoice = true;
				break;
			}
		}
		if (sameChoice)
			break;
	}

	if (numOfJoinedPlayers > 1 && !sameChoice)
	{
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			if (!selectionInfoMap[q].joinedGame || !selectionInfoMap[q].ready)
				GameInfo::GetInstance()->RemovePlayer(q);
			else
			{
				switch (selectionInfoMap[q].characterChoice)
				{
                case CHOICE_AVERRA:
                    q->SetCharacter(Create::playerRangeEntity(EntityManager::GetInstance(), "player_staff", 160.0f, 16.0f, 0.25f, 0.6f, Vector3(50.0f, 22.0f, 0), Vector3(3, 3, 1)));
                    break;
                case CHOICE_ENNA:
                    q->SetCharacter(Create::playerMeleeEntity(EntityManager::GetInstance(), "player_shield", 220.0f, 20.0f, 0.4f, 1.2f, Vector3(50.0f, 22.0f, 0), Vector3(3, 3, 1)));
                    break;

				default:break;
				}
			}
		}

		GameInfo::GetInstance()->Init();

		switch (GameInfo::GetInstance()->GetGameMode())
		{
		case GameInfo::GAMEMODE_STORY:
			EntityManager::GetInstance()->ClearList();
			SceneManager::GetInstance()->SetActiveScene("SceneScrolling");
			break;
		case GameInfo::GAMEMODE_ENDLESS:
			EntityManager::GetInstance()->ClearList();
			SceneManager::GetInstance()->SetActiveScene("SceneEndless");
			break;
		case GameInfo::GAMEMODE_TUTORIAL:
			EntityManager::GetInstance()->ClearList();
			SceneManager::GetInstance()->SetActiveScene("SceneTutorial");
			break;
		default:
			break;
		}
	}
}

void SelectionState::Render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//GraphicsManager::GetInstance()->UpdateLightUniforms();

	// Setup 3D pipeline then render 3D
	float halfOrthoWidth = orthoWidth * 0.5f;
	float halfOrthoHeight = orthoHeight * 0.5f;
	GraphicsManager::GetInstance()->SetOrthographicProjection(-halfOrthoWidth, halfOrthoWidth, -halfOrthoHeight, halfOrthoHeight, -1000, 1000);
	GraphicsManager::GetInstance()->AttachCamera(camera);
	entityManager->Render();

	// Setup 2D pipeline then render 2D
	GraphicsManager::GetInstance()->SetOrthographicProjection(
		0, canvasWidth,		// minX - maxX
		0, canvasHeight,	// minY - maxY
		-10, 10);											// minZ - maxZ
	GraphicsManager::GetInstance()->DetachCamera();
	entityManager->RenderUI();
}

void SelectionState::Exit()
{
	SceneBase2D::Exit();

	//MeshBuilder::GetInstance()->RemoveMesh("backBtn");
	//MeshBuilder::GetInstance()->RemoveMesh("choice_attacker");
	//MeshBuilder::GetInstance()->RemoveMesh("choice_defender");

	for (auto q : GameInfo::GetInstance()->GetPlayersList())
	{
		if (!selectionInfoMap[q].joinedGame || !selectionInfoMap[q].ready)
			GameInfo::GetInstance()->RemovePlayer(q);
	}

	if (entityManager)
	{
		entityManager->ClearList();
	}
	//for (int i = 0; i < CHOICE_END; ++i)
	//{
	//	if (charaPortrait[i])
	//	{
	//		delete charaPortrait[i];
	//		charaPortrait[i] = NULL;
	//	}
	//}

	if (backBtn)
		delete backBtn;
}

//#include "SelectionState.h"
//#include "GL\glew.h"
//
//#include "..\Application.h"
//#include "LoadTGA.h"
//#include "GraphicsManager.h"
//#include "MeshBuilder.h"
//#include "..\TextEntity.h"
//#include "RenderHelper.h"
//
//#include "KeyboardController.h"
//#include "MouseController.h"
//#include "SceneManager.h"
//#include "..\SP4\GameInfo.h"
//
//#include "..\PlayerInfo\PlayerRangeEntity.h"
//#include "..\PlayerInfo\PlayerMeleeEntity.h"
//
//SelectionState::SelectionState()
//{
//	entityManager = new EntityManager();
//	for (int i = 0; i < CHOICE_END; ++i)
//		charaPortrait[i] = NULL;
//}
//
//SelectionState::~SelectionState()
//{
//	if (entityManager)
//	{
//		entityManager->ClearList();
//		delete entityManager;
//		entityManager = NULL;
//	}
//}
//
//void SelectionState::Init()
//{
//	SceneBase2D::Init();
//	
//	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
//	Application::SetCursorMode2D();
//
// 	MeshBuilder::GetInstance()->GenerateQuad("backBtn", Color(1.0f, 1.0f, 0.0f), 1.0f);
//	MeshBuilder::GetInstance()->GetMesh("backBtn")->textureID = LoadTGA("Image//playBtn.tga");
//
//	charaPortrait[CHOICE_ATTACKER] = MeshBuilder::GetInstance()->GenerateQuad("choice_attacker", Color(1, 0, 0), 1.0f);
//	MeshBuilder::GetInstance()->GetMesh("choice_attacker")->textureID = LoadTGA("Image//charaselect_attacker.tga");
//	charaPortrait[CHOICE_DEFENDER] = MeshBuilder::GetInstance()->GenerateQuad("choice_defender", Color(0, 0, 1), 1.0f);
//	MeshBuilder::GetInstance()->GetMesh("choice_defender")->textureID = LoadTGA("Image//charaselect_defender.tga");
//
//	canvasWidth = (float)Application::GetInstance().GetWindowWidth();
//	canvasHeight = (float)Application::GetInstance().GetWindowHeight();
//	orthoWidth = canvasWidth;
//	orthoHeight = canvasHeight;
//	worldWidth = orthoWidth;
//	worldHeight = orthoHeight;
//
//	float halfWorldWidth = orthoWidth * 0.5f;
//	float halfWorldHeight = orthoHeight * 0.5f;
//
//	camera->Init(Vector3(halfWorldWidth, halfWorldHeight, 1), Vector3(halfWorldWidth, halfWorldHeight, 0), Vector3(0, 1, 0));
//
//	//Create::Entity(entityManager, "background", Vector3(halfWorldWidth, halfWorldHeight, -1.0f), Vector3(orthoWidth, orthoHeight, 1));
//	backBtn = Create::button(entityManager, "backBtn", Vector3(canvasWidth*0.1f, canvasHeight * 0.95f, 0.0f), Vector3(150, 45, 1));
//	startBtn = Create::button(entityManager, "backBtn", Vector3(canvasWidth*0.5f, canvasHeight * 0.05f, 0.0f), Vector3(180, 54, 1));
//
//	GameInfo::GetInstance()->AddPlayer(new PlayerInfo());
//	GameInfo::GetInstance()->AddPlayer(new PlayerInfo());
//	GameInfo::GetInstance()->AddPlayer(new PlayerInfo());
//	GameInfo::GetInstance()->GetPlayersList()[0]->LoadControls("Player1_Controls");
//	GameInfo::GetInstance()->GetPlayersList()[1]->LoadControls("Player2_Controls");
//
//	for (auto q : GameInfo::GetInstance()->GetPlayersList())
//	{
//		selectionInfoMap[q].portrait = Create::Entity(entityManager, "choice_attacker", Vector3());
//
//		selectionInfoMap[q].characterChoice = 0;
//		selectionInfoMap[q].joinedGame = false;
//		selectionInfoMap[q].portrait->SetShouldRender(false);
//	}
//
//	//GameInfo::GetInstance()->GetPlayersList().front()->SetCharacter(Create::playerRangeEntity(EntityManager::GetInstance(), "player_staff", 10.0f, 5.0f, 0.5f, 1.0f, Vector3(50.0f, 22.0f, 0), Vector3(3, 3, 1)));
//	//GameInfo::GetInstance()->GetPlayersList().back()->SetCharacter(Create::playerMeleeEntity(EntityManager::GetInstance(), "player_shield", 10.0f, 5.0f, 0.5f, 1.0f, Vector3(50.0f, 22.0f, 0), Vector3(3, 3, 1)));
//}
//
//void SelectionState::UpdateInputs(double dt)
//{
//	SceneBase2D::UpdateInputs(dt);
//	float mouseX, mouseY;
//	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
//	mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
//	mouseX *= canvasWidth / Application::GetInstance().GetWindowWidth();
//	mouseY *= canvasHeight / Application::GetInstance().GetWindowHeight();
//	Vector3 mousePos(mouseX, mouseY, 0);
//
//	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
//	{
//		if (backBtn->CheckCollision(mousePos))
//		{
//			SceneManager::GetInstance()->SetActiveScene("MenuState");
//		}
//		else if(startBtn->CheckCollision(mousePos))
//		{
//			StartGame();
//		}
//	}
//	if (KeyboardController::GetInstance()->IsKeyPressed(VK_RETURN))
//	{
//		StartGame();
//	}
//	if (KeyboardController::GetInstance()->IsKeyPressed(VK_ESCAPE) || KeyboardController::GetInstance()->IsKeyPressed(VK_BACK))
//	{
//		SceneManager::GetInstance()->SetActiveScene("GameModeSelectionState");
//	}
//	for (auto q : GameInfo::GetInstance()->GetPlayersList())
//	{
//		if (KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_SPECIAL)))
//		{
//			selectionInfoMap[q].joinedGame = !selectionInfoMap[q].joinedGame;
//			selectionInfoMap[q].portrait->SetShouldRender(selectionInfoMap[q].joinedGame);
//			
//			int counter = 0;
//			int numOfJoinedPlayers = 0;
//			for (auto r : GameInfo::GetInstance()->GetPlayersList())
//			{
//				if (selectionInfoMap[r].joinedGame)
//					numOfJoinedPlayers++;
//			}
//			float size = (canvasWidth * 0.9f) / numOfJoinedPlayers;
//			size = Math::Min(size, canvasHeight * 0.8f);
//			for (auto r : GameInfo::GetInstance()->GetPlayersList())
//			{
//				if (selectionInfoMap[r].joinedGame == false)
//					continue;
//				if (numOfJoinedPlayers == 1)
//					selectionInfoMap[r].portrait->SetPosition(Vector3(canvasWidth * 0.5f, canvasHeight * 0.5f, 0));
//				else
//					selectionInfoMap[r].portrait->SetPosition(Vector3(((canvasWidth * 0.05f) + (size * 0.5f)) + (counter * size), canvasHeight * 0.5f, 0));
//				selectionInfoMap[r].portrait->SetScale(Vector3(size, size, 1));
//
//				counter++;
//			}
//		}
//		if (selectionInfoMap[q].joinedGame)
//		{
//			if (KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_MOVELEFT)))
//			{
//				selectionInfoMap[q].characterChoice = abs((selectionInfoMap[q].characterChoice - 1) % CC_END);
//				selectionInfoMap[q].portrait->SetMesh(charaPortrait[selectionInfoMap[q].characterChoice]);
//			}
//			else if (KeyboardController::GetInstance()->IsKeyPressed(q->GetKey(PlayerInfo::HOTKEY_MOVERIGHT)))
//			{
//				selectionInfoMap[q].characterChoice = (selectionInfoMap[q].characterChoice + 1) % CC_END;
//				selectionInfoMap[q].portrait->SetMesh(charaPortrait[selectionInfoMap[q].characterChoice]);
//			}
//		}
//	}
//}
//
//void SelectionState::Update(double dt)
//{
//	//for (auto q : GameInfo::GetInstance()->GetPlayersList())
//	//{
//	//	std::cout << selectionInfoMap[q].joinedGame << " " << selectionInfoMap[q].characterChoice << "     ";
//	//}
//	//std::cout << std::endl;
//}
//
//void SelectionState::StartGame()
//{
//	int numOfJoinedPlayers = 0;
//	for (auto q : GameInfo::GetInstance()->GetPlayersList())
//	{
//		if (selectionInfoMap[q].joinedGame)
//			numOfJoinedPlayers++;
//	}
//	bool sameChoice = false;
//	for (auto q : selectionInfoMap)
//	{
//		if (!q.second.joinedGame)
//			continue;
//		for (auto r : selectionInfoMap)
//		{
//			if (!r.second.joinedGame)
//				continue;
//			if (q.first == r.first)
//				continue;
//
//			if (q.second.characterChoice == r.second.characterChoice)
//			{
//				sameChoice = true;
//				break;
//			}
//		}
//		if (sameChoice)
//			break;
//	}
//
//	if (numOfJoinedPlayers > 1 && !sameChoice)
//	{
//		for (auto q : GameInfo::GetInstance()->GetPlayersList())
//		{
//			if (selectionInfoMap[q].joinedGame)
//			{
//				switch (selectionInfoMap[q].characterChoice)
//				{
//				case CHOICE_ATTACKER:
//					q->SetCharacter(Create::playerRangeEntity(EntityManager::GetInstance(), "player_staff", 200.0f, 20.0f, 0.5f, 1.0f, Vector3(50.0f, 22.0f, 0), Vector3(3, 3, 1)));
//					break;
//				case CHOICE_DEFENDER:
//					q->SetCharacter(Create::playerMeleeEntity(EntityManager::GetInstance(), "player_shield", 200.0f, 20.0f, 0.5f, 1.0f, Vector3(50.0f, 22.0f, 0), Vector3(3, 3, 1)));
//					break;
//
//				default:break;
//				}
//			}
//			else
//				GameInfo::GetInstance()->RemovePlayer(q);
//		}
//
//		GameInfo::GetInstance()->Init();
//		SceneManager::GetInstance()->SetActiveScene("SceneScrolling");
//	}
//}
//
//void SelectionState::Render()
//{
//	SceneBase2D::Render();
//}
//
//void SelectionState::Exit()
//{
//	SceneBase2D::Exit();
//
//	//MeshBuilder::GetInstance()->RemoveMesh("backBtn");
//	//MeshBuilder::GetInstance()->RemoveMesh("choice_attacker");
//	//MeshBuilder::GetInstance()->RemoveMesh("choice_defender");
//
//	if (entityManager)
//	{
//		entityManager->ClearList();
//	}
//	//for (int i = 0; i < CHOICE_END; ++i)
//	//{
//	//	if (charaPortrait[i])
//	//	{
//	//		delete charaPortrait[i];
//	//		charaPortrait[i] = NULL;
//	//	}
//	//}
//	
//	if (backBtn)
//		delete backBtn;
//	if (startBtn)
//		delete startBtn;
//}