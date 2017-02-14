#include "SceneManager.h"
#include "Scene.h"
#include "GL\glew.h"

SceneManager::SceneManager() : nextScene(nullptr)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Update(double _dt)
{
	if (sceneStack.size() > 0)
	{
		//// Check for change of scene
		//if (nextScene != sceneStack.back())
		//{
		//	if (sceneStack.back())
		//	{
		//		// Scene is valid, need to call appropriate function to exit
		//		sceneStack.back()->Exit();
		//	}
		//	sceneStack.pop_back();
		//	sceneStack.push_back(nextScene);
		//	sceneStack.back()->Init();
		//}

		//only update inputs of closest overlay
		sceneStack.back()->UpdateInputs(_dt);
		for (auto q : sceneStack)
		{
			q->Update(_dt);
		}
	}
	else if (nextScene)
	{
		sceneStack.push_back(nextScene);
		sceneStack.back()->Init();
	}
}

void SceneManager::Render()
{
	if (sceneStack.size() > 0)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//sceneStack.back()->Render();
		std::list<Scene*>::reverse_iterator rit;
		for (rit = sceneStack.rbegin(); rit != sceneStack.rend(); ++rit)
		{
			(*rit)->Render();
		}
		//for (auto q : sceneStack)
		//{
		//	q->Render();
		//}
	}
}

void SceneManager::Exit()
{
	// Delete all scenes stored and empty the entire map
	for (auto q : sceneStack)
	{
		q->Exit();
	}
	std::map<std::string, Scene*>::iterator it, end;
	end = sceneMap.end();
	for (it = sceneMap.begin(); it != end; ++it)
	{
		delete it->second;
	}
	sceneMap.clear();
}

void SceneManager::AddScene(const std::string& _name, Scene* _scene)
{
	if (CheckSceneExist(_name))
	{
		// Scene Exist, unable to proceed
		throw std::exception("Duplicate scene name provided");
	}

	if (_scene == nullptr)
	{
		throw std::invalid_argument("Invalid scene pointer");
	}

	// Nothing wrong, add the scene to our map
	sceneMap[_name] = _scene;
}

void SceneManager::RemoveScene(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!CheckSceneExist(_name))
		return;

	Scene* target = sceneMap[_name];
	if (target == sceneStack.back() || target == nextScene)
	{
		throw std::exception("Unable to remove active/next scene");
	}

	// Delete and remove from our map
	delete target;
	sceneMap.erase(_name);
}

void SceneManager::OverlayScene(const std::string& _name)
{
	if (!CheckSceneExist(_name))
	{
		// Scene does not exist
		throw std::exception("Scene does not exist");
	}

	// Scene exist, set the next scene pointer to that scene
	//nextScene = sceneMap[_name];
	sceneMap[_name]->Init();
	sceneStack.push_back(sceneMap[_name]);
}

void SceneManager::SetActiveScene(const std::string& _name)
{
	if (!CheckSceneExist(_name))
	{
		// Scene does not exist
		throw std::exception("Scene does not exist");
	}

	while (sceneStack.size() > 0)
	{
		sceneStack.back()->Exit();
		sceneStack.pop_back();
	}
	// Scene exist, set the next scene pointer to that scene
	nextScene = sceneMap[_name];
}

void SceneManager::ExitOverlayScene()
{
	if (sceneStack.size() > 1)
	{
		sceneStack.back()->Exit(); 
		sceneStack.pop_back();
		//nextScene->Exit();
		//sceneStack.pop_back();
		//nextScene = sceneStack.back();
	}
}

bool SceneManager::CheckSceneExist(const std::string& _name)
{
	return sceneMap.count(_name) != 0;
}