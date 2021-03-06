#include "Application.h"
#include "MouseController.h"
#include "KeyboardController.h"
#include "SceneManager.h"
#include "GraphicsManager.h"
#include "SoundManager.h"
#include "ShaderProgram.h"
#include "Lua\LuaInterface.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "SceneText2.h"
#include "GameStateManagement\IntroState.h"
#include "GameStateManagement\MenuState.h"
#include "GameStateManagement\SelectionState.h"
#include "GameStateManagement\PauseState.h"
#include "GameStateManagement\GameModeSelectionState.h"
#include "SP4\SceneScrolling.h"
#include "SP4\SceneBoss.h"
#include "SP4\SceneTutorial.h"
#include "SP4\SceneBoss2.h"
#include "SP4\SceneEndless.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	Application::GetInstance().m_window_width = w;
	Application::GetInstance().m_window_height = h;
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::IsFocused()
{
	return glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
}

Application::Application():
m_window_width(640),
m_window_height(480),
closeProgram(false)
{
}

Application::~Application()
{
}

void Application::InitDisplay(void)
{
	currProg = GraphicsManager::GetInstance()->LoadShader("default", "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader");

	// Tell the shader program to store these uniform locations
	currProg->AddUniform("MVP");
	currProg->AddUniform("MV");
	currProg->AddUniform("MV_inverse_transpose");
	currProg->AddUniform("material.kAmbient");
	currProg->AddUniform("material.kDiffuse");
	currProg->AddUniform("material.kSpecular");
	currProg->AddUniform("material.kShininess");
	currProg->AddUniform("lightEnabled");
	currProg->AddUniform("numLights");
	currProg->AddUniform("lights[0].type");
	currProg->AddUniform("lights[0].position_cameraspace");
	currProg->AddUniform("lights[0].color");
	currProg->AddUniform("lights[0].power");
	currProg->AddUniform("lights[0].kC");
	currProg->AddUniform("lights[0].kL");
	currProg->AddUniform("lights[0].kQ");
	currProg->AddUniform("lights[0].spotDirection");
	currProg->AddUniform("lights[0].cosCutoff");
	currProg->AddUniform("lights[0].cosInner");
	currProg->AddUniform("lights[0].exponent");
	currProg->AddUniform("lights[1].type");
	currProg->AddUniform("lights[1].position_cameraspace");
	currProg->AddUniform("lights[1].color");
	currProg->AddUniform("lights[1].power");
	currProg->AddUniform("lights[1].kC");
	currProg->AddUniform("lights[1].kL");
	currProg->AddUniform("lights[1].kQ");
	currProg->AddUniform("lights[1].spotDirection");
	currProg->AddUniform("lights[1].cosCutoff");
	currProg->AddUniform("lights[1].cosInner");
	currProg->AddUniform("lights[1].exponent");
	currProg->AddUniform("colorTextureEnabled");
	currProg->AddUniform("colorTexture");
	currProg->AddUniform("textEnabled");
	currProg->AddUniform("textColor");

	// Tell the graphics manager to use the shader we just loaded
	GraphicsManager::GetInstance()->SetActiveShader("default");
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	LuaInterface::GetInstance()->Init();
	m_window_width = LuaInterface::GetInstance()->GetIntValue("ApplicationSettings", "width");
	m_window_height = LuaInterface::GetInstance()->GetIntValue("ApplicationSettings", "height");

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "FlapFlap Hunters", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide the cursor
	glfwSetMouseButtonCallback(m_window, &Application::MouseButtonCallbacks);
	glfwSetScrollCallback(m_window, &Application::MouseScrollCallbacks);

	// Init systems
	GraphicsManager::GetInstance()->Init();
	InitDisplay();
	SoundManager::GetInstance()->Init();

	SceneManager::GetInstance()->AddScene("IntroState", new IntroState());
	SceneManager::GetInstance()->AddScene("MenuState", new MenuState());
	SceneManager::GetInstance()->AddScene("GameModeSelectionState", new GameModeSelectionState());
	SceneManager::GetInstance()->AddScene("SelectionState", new SelectionState());
	SceneManager::GetInstance()->AddScene("PauseState", new PauseState());
	SceneManager::GetInstance()->AddScene("SceneScrolling", new SceneScrolling());
	SceneManager::GetInstance()->AddScene("SceneBoss", new SceneBoss());
	SceneManager::GetInstance()->AddScene("SceneTutorial", new SceneTutorial());
	SceneManager::GetInstance()->AddScene("SceneBoss2", new SceneBoss2());
    SceneManager::GetInstance()->AddScene("SceneEndless", new SceneEndless());
	
	SceneManager::GetInstance()->SetActiveScene("IntroState");
}

void Application::Run()
{
	//sfx
	SoundManager::GetInstance()->LoadMedia("player_range_attack", "Media//Range Player Attack.wav");
	SoundManager::GetInstance()->LoadMedia("player_melee_attack", "Media//Melee Player Attack.wav");
	SoundManager::GetInstance()->LoadMedia("player_melee_shield", "Media//Player Shield Sound.wav");
	SoundManager::GetInstance()->LoadMedia("floopy_attack", "Media//Enemy Sound.wav");
	SoundManager::GetInstance()->LoadMedia("bitey_attack", "Media//Player Range Attack(2).wav");
	SoundManager::GetInstance()->LoadMedia("skywhale_chargeLaser", "Media//Boss Charging.wav");
	SoundManager::GetInstance()->LoadMedia("skywhale_fireLaser", "Media//Boss Lazer Sound.wav");
	//bgm
	SoundManager::GetInstance()->LoadBGM("bossfight", "Media//Normal Boss Fight sound.wav");
	SoundManager::GetInstance()->LoadBGM("bossfight_angry", "Media//Angry Boss Fight Sound.wav");

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	SceneManager* ttt = SceneManager::GetInstance();

	while (!glfwWindowShouldClose(m_window) && !closeProgram)
	{
		glfwPollEvents();
		if (IsFocused())
			UpdateInput();
		
		ttt->Update(m_timer.getElapsedTime());
		SceneManager::GetInstance()->Render();

		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...

        m_timer.waitUntil(frameTime);// Frame rate limiter. Limits each frame to a specified time in ms.   
		if (IsFocused())
			PostInputUpdate();
	}

	SceneManager::GetInstance()->Exit();
	SoundManager::GetInstance()->Destroy();
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

void Application::UpdateInput()
{
	// Update Mouse Position
	double mouse_currX, mouse_currY;
	glfwGetCursorPos(m_window, &mouse_currX, &mouse_currY);
	MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);

	// Update Keyboard Input
	for (int i = 0; i < KeyboardController::MAX_KEYS; ++i)
		KeyboardController::GetInstance()->UpdateKeyboardStatus(i, IsKeyPressed(i));
}

void Application::PostInputUpdate()
{
	// If mouse is centered, need to update the center position for next frame
	if (MouseController::GetInstance()->GetKeepMouseCentered())
	{
		double mouse_currX, mouse_currY;
		mouse_currX = m_window_width >> 1;
		mouse_currY = m_window_height >> 1;
		MouseController::GetInstance()->UpdateMousePosition(mouse_currX, mouse_currY);
		glfwSetCursorPos(m_window, mouse_currX, mouse_currY);
	}

	// Call input systems to update at end of frame
	MouseController::GetInstance()->EndFrameUpdate();
	KeyboardController::GetInstance()->EndFrameUpdate();
}

void Application::MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods)
{
	// Send the callback to the mouse controller to handle
	if (action == GLFW_PRESS)
		MouseController::GetInstance()->UpdateMouseButtonPressed(button);
	else
		MouseController::GetInstance()->UpdateMouseButtonReleased(button);
}

void Application::MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset)
{
	MouseController::GetInstance()->UpdateMouseScroll(xoffset, yoffset);
}

void Application::HideCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	Application::GetInstance().SetIsCursorShown(false);
}

void Application::ShowCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	Application::GetInstance().SetIsCursorShown(true);
}

void Application::LockCursor()
{
	MouseController::GetInstance()->SetKeepMouseCentered(true);
}
void Application::UnlockCursor()
{
	MouseController::GetInstance()->SetKeepMouseCentered(false);
}
void Application::SetCursorMode2D()
{
	ShowCursor();
	UnlockCursor();
}
void Application::SetCursorMode3D()
{
	HideCursor();
	LockCursor();
}

bool Application::GetisCursorLocked()
{
	return MouseController::GetInstance()->GetKeepMouseCentered();
}

void Application::Close()
{
	Application::GetInstance().SetCloseProgram(true);
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}