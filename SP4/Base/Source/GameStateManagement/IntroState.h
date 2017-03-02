#ifndef INTRO_SCENE_H
#define INTRO_SCENE_H

#include "..\SceneBase2D.h"

#include "..\Button.h"

#define NUM_PAGES 4
#define PAGE_FLIP_SPEED 40.0f

class Mesh;

struct PageInfo
{
	float flipValue;
	bool flipping;
	int flipDirection;
	Mesh* mesh;
};

class IntroState : public SceneBase2D
{
public:
	IntroState();
	virtual ~IntroState();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	void NextPage();
	void PrevPage();

	PageInfo pages[NUM_PAGES];

	Button* skipBtn;
	Button* nextPageBtn;
	Button* prevPageBtn;
};

#endif