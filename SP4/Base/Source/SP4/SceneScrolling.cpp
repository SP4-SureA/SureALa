#include "SceneScrolling.h"
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

SceneScrolling::SceneScrolling()
{
}

SceneScrolling::~SceneScrolling()
{
}

void SceneScrolling::Init()
{
	SceneBase2D::Init();
	Application::ShowCursor();

	//MeshBuilder::GetInstance()->GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	//MeshBuilder::GetInstance()->GetMesh("bg")->textureID = LoadTGA("Image//background.tga");

	MeshBuilder::GetInstance()->GenerateQuad("quad", Color(1, 1, 1), 1.f);

	orthoHeight = 100;
	orthoWidth = orthoHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());

	float halfWorldWidth = worldWidth * 0.5f;
	float halfWorldheight = worldHeight * 0.5f;

	camera.Init(Vector3(halfWorldWidth, halfWorldheight, 1), Vector3(halfWorldWidth, halfWorldheight, 0), Vector3(0, 1, 0));

	groundEntity = Create::Entity(entityManager, "quad", Vector3(halfWorldWidth, worldHeight * 0.35f, -1), Vector3(worldWidth*0.7f, worldHeight*0.7f, 0));
}

void SceneScrolling::UpdateInputs(double dt)
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

void SceneScrolling::Update(double dt)
{
	orthoWidth = orthoHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());
	Client::GetInstance()->Update(dt);
	SceneBase2D::Update(dt);
	NetworkEntityManager::GetInstance()->UpdateInterpolations(dt);

	Vector3 min = -(groundEntity->GetScale() * 0.5f) + groundEntity->GetPosition();
	Vector3 max = (groundEntity->GetScale() * 0.5f) + groundEntity->GetPosition();
	entityManager->HandleOutOfBounds(min, max, dt);

}

void SceneScrolling::Render()
{
	SceneBase2D::Render();
}

void SceneScrolling::Exit()
{
	SceneBase2D::Exit();

	entityManager->ClearList();
}