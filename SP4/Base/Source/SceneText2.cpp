#include "SceneText2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "KeyboardController.h"
#include "MouseController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "EntityManager.h"

#include "GenericEntity.h"
#include "GroundEntity.h"
#include "TextEntity.h"
#include "SpriteEntity.h"
#include "Light.h"
#include "SkyBox/SkyBoxEntity.h"

#include <iostream>
using namespace std;

SceneText2::SceneText2()
{
}

SceneText2::~SceneText2()
{
}

void SceneText2::Init()
{
	SceneBase2D::Init();

	orthoHeight;
	orthoWidth;
	worldHeight = 100;
	worldWidth = worldHeight * ((float)Application::GetInstance().GetWindowWidth() / Application::GetInstance().GetWindowHeight());

	// Create the playerinfo instance, which manages all information about the player
	//playerInfo = CPlayerInfo::GetInstance();
	//playerInfo->Init();

	// Create entities into the scene
	Create::Entity(entityManager, "axis", Vector3(0.0f, 0.0f, 0.0f)); // Reference
	Create::Entity(entityManager, "lightball", Vector3(lights[0]->position.x, lights[0]->position.y, lights[0]->position.z)); // Lightball
	GenericEntity* aCube = Create::Entity(entityManager, "cube", Vector3(-20.0f, 0.0f, -20.0f));

}

void SceneText2::Update(double dt)
{
	SceneBase2D::Update(dt);

	// if the left mouse button was released
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::LMB))
	{
		cout << "Left Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::RMB))
	{
		cout << "Right Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->IsButtonReleased(MouseController::MMB))
	{
		cout << "Middle Mouse Button was released!" << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in X-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_XOFFSET) << endl;
	}
	if (MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) != 0.0)
	{
		cout << "Mouse Wheel has offset in Y-axis of " << MouseController::GetInstance()->GetMouseScrollStatus(MouseController::SCROLL_TYPE_YOFFSET) << endl;
	}
}

void SceneText2::Render()
{
	SceneBase2D::Render();
}

void SceneText2::Exit()
{
	//playerInfo->DetachCamera();

	//playerInfo->Destroy();

	SceneBase2D::Exit();
}
