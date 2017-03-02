#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class ShaderProgram;
struct GLFWwindow;

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void InitDisplay(void);
	void Run();
	void Exit();

	void UpdateInput();
	void PostInputUpdate();
	
	static void MouseButtonCallbacks(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallbacks(GLFWwindow* window, double xoffset, double yoffset);
	
	static bool IsFocused();

	static void HideCursor();
	static void ShowCursor();
	static void LockCursor();
	static void UnlockCursor();
	static void SetCursorMode2D();
	static void SetCursorMode3D();

	static bool GetisCursorLocked();

	static void Close();

	int GetWindowHeight();
	int GetWindowWidth();

	// Should make these not hard-coded :P
	int m_window_width;
	int m_window_height;

	inline void SetCloseProgram(bool _close){ this->closeProgram = _close; }
	
	inline void SetIsCursorShown(bool _show){ this->isCursorShown = _show; }
	inline bool GetIsCursorShown(){ return this->isCursorShown; }

private:
	Application();
	~Application();

	static bool IsKeyPressed(unsigned short key);

	//Declare a window object
	StopWatch m_timer;
	ShaderProgram* currProg;
	bool closeProgram;

	bool isCursorShown;
};

#endif