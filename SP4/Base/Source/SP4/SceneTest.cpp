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

SceneTest::SceneTest()
{
}

SceneTest::~SceneTest()
{
}

void SceneTest::Init()
{
	SceneBase2D::Init();

	camera.Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	GraphicsManager::GetInstance()->AttachCamera(&camera);
	Application::ShowCursor();

	//MeshBuilder::GetInstance()->GenerateQuad("bg", Color(1, 1, 1), 1.0f);
	//MeshBuilder::GetInstance()->GetMesh("bg")->textureID = LoadTGA("Image//background.tga");

	orthoHeight = 100;
	orthoWidth = orthoHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());
	worldHeight = 100;
	worldWidth = worldHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());

	float halfWorldWidth = worldWidth * 0.5f;
	float halfWorldheight = worldHeight * 0.5f;

	camera.Init(Vector3(halfWorldWidth, halfWorldheight, 1), Vector3(halfWorldWidth, halfWorldheight, 0), Vector3(0, 1, 0));

	testy = Create::Entity(entityManager, "circle", Vector3(halfWorldWidth, halfWorldheight, 0));
	testy2 = Create::Entity(entityManager, "circle", Vector3(halfWorldWidth * 1.2f, halfWorldheight*1.01, 0));
	testy2->SetVelocity(Vector3(-5, 0, 0));
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

	if (KeyboardController::GetInstance()->IsKeyPressed('V'))
		testy->SetVelocity(Vector3(15, 0, 0));
}

void SceneTest::Update(double dt)
{
	SceneBase2D::Update(dt);

	if (Physics::CheckCollision(testy, testy2, dt))
	{
		Physics::CollisionResponse(testy, testy2);
	}
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