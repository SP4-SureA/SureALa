#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "SingletonTemplate.h"
#include <map>
#include <string>
#include <list>

class Scene;

class SceneManager : public Singleton<SceneManager>
{
	friend Singleton<SceneManager>;
public:
	// System Interface
	void Update(double _dt);
	void Render();
	void Exit();

	// User Interface
	void AddScene(const std::string& _name, Scene* _scene);
	void RemoveScene(const std::string& _name);
	void OverlayScene(const std::string& _name);
	void SetActiveScene(const std::string& _name);
	bool CheckSceneExist(const std::string& _name);

	void ExitOverlayScene();

private:
	SceneManager();
	~SceneManager();

	std::map<std::string, Scene*> sceneMap;
	Scene* nextScene;
	std::list<Scene*> sceneStack;
};

#endif // SCENE_MANAGER_H