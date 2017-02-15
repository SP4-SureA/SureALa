#include "SceneTest.h"
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
#include "..\RakNet\Client.h"

#include "..\PlayerInfo\PlayerEntityBase.h"

//testy
#include "..\RakNet\NetworkEntityManager.h"

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

void SceneTest::Init()
{
	SceneBase2D::Init();
	Application::ShowCursor();

	//MeshBuilder::GetInstance()->GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	//MeshBuilder::GetInstance()->GetMesh("bg")->textureID = LoadTGA("Image//background.tga");

	float halfWorldWidth = worldWidth * 0.5f;
	float halfWorldheight = worldHeight * 0.5f;

	camera.Init(Vector3(halfWorldWidth, halfWorldheight, 1), Vector3(halfWorldWidth, halfWorldheight, 0), Vector3(0, 1, 0));
}

void SceneTest::UpdateInputs(double dt)
{
	SceneBase2D::UpdateInputs(dt);
	float mouseX, mouseY;
	MouseController::GetInstance()->GetMousePosition(mouseX, mouseY);
	mouseY = Application::GetInstance().GetWindowHeight() - mouseY;
	mouseX = (mouseX / Application::GetInstance().GetWindowWidth()) * orthoWidth + (camera.GetCameraPos().y - (orthoHeight*0.5f));
	mouseY = (mouseY / Application::GetInstance().GetWindowHeight()) * orthoHeight + (camera.GetCameraPos().x - (orthoWidth*0.5f));
	Vector3 mousePos(mouseX, mouseY, 0);

	PlayerInfo::GetInstance()->GetCharacter()->UpdateInputs(dt);
}

void SceneTest::Update(double dt)
{
	Client::GetInstance()->Update(dt);
	SceneBase2D::Update(dt);
	NetworkEntityManager::GetInstance()->UpdateInterpolations(dt);
}

void SceneTest::Render()
{
	SceneBase2D::Render();
}

void SceneTest::Exit()
{
	SceneBase2D::Exit();

	entityManager->ClearList();
}