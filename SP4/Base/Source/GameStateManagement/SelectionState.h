#ifndef SELECTION_SCREEN_H
#define SELECTION_SCREEN_H

#include "..\SceneBase2D.h"

#include "..\Button.h"
#include "..\GenericEntity.h"

#include <map>
using std::map;

class PlayerInfo;
class TextEntity;

struct SelectionInfo
{
	int characterChoice;
	bool joinedGame;
	bool ready;
	GenericEntity* portrait;
};

struct PortraitInfo
{
	Mesh* selectedMesh;
	Mesh* notSelectedMesh;

	bool selected;
};

class SelectionState : public SceneBase2D
{
public:
	enum SELECTION_CHOICE
	{
		CHOICE_START = -1,

		CHOICE_AVERRA,
		CHOICE_ENNA,

		CHOICE_END
	};
	SelectionState();
	~SelectionState();

	virtual void Init();
	virtual void UpdateInputs(double dt);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

protected:
	void StartGame();

	PortraitInfo charaPortrait[CHOICE_END];
	std::map<PlayerInfo*, SelectionInfo> selectionInfoMap;
	Button* backBtn;
	TextEntity* textEntity;
	float gameStartTimer;
};



#endif


//#ifndef SELECTION_SCREEN_H
//#define SELECTION_SCREEN_H
//
//#include "..\SceneBase2D.h"
//
//#include "..\Button.h"
//#include "..\GenericEntity.h"
//
//#include <map>
//using std::map;
//
//class PlayerInfo;
//
//struct SelectionInfo
//{
//	int characterChoice;
//	bool joinedGame;
//	GenericEntity* portrait;
//};
//
//enum CHARACTER_CHOICE
//{
//	CC_ATTACKER,
//	CC_DEFENDER,
//
//	CC_END
//};
//
//class SelectionState : public SceneBase2D
//{
//public:
//	enum SELECTION_CHOICE
//	{
//		CHOICE_START = -1,
//
//		CHOICE_ATTACKER,
//		CHOICE_DEFENDER,
//
//		CHOICE_END
//	};
//	SelectionState();
//	~SelectionState();
//
//	virtual void Init();
//	virtual void UpdateInputs(double dt);
//	virtual void Update(double dt);
//	virtual void Render();
//	virtual void Exit();
//
//protected:
//	void StartGame();
//
//	Mesh* charaPortrait[CHOICE_END];
//	std::map<PlayerInfo*, SelectionInfo> selectionInfoMap;
//	Button* backBtn;
//	Button* startBtn;
//};
//
//
//
//#endif