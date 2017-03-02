#include "PlayerInfo.h"

#include "PlayerEntityBase.h"
#include "..\Lua\LuaInterface.h"

#include "KeyboardController.h"

PlayerInfo::PlayerInfo()
{
	key[HOTKEY_MOVEUP] = 'I',
	key[HOTKEY_MOVEDOWN] = 'K';
	key[HOTKEY_MOVELEFT] = 'J';
	key[HOTKEY_MOVERIGHT] = 'L';
	key[HOTKEY_SHOOTUP] = 101;
	key[HOTKEY_SHOOTDOWN] = 98;
	key[HOTKEY_SHOOTLEFT] = 97;
	key[HOTKEY_SHOOTRIGHT] = 99;
	key[HOTKEY_SPECIAL] = 'Q';
}

PlayerInfo::~PlayerInfo()
{
}

void PlayerInfo::UpdateInputs(double dt)
{
	character->SetMoveDir(Vector3());
	character->SetShootDir(Vector3());

	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_MOVEUP]))
	{
		character->AddMoveDir(Vector3(0, 1, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_MOVEDOWN]))
	{
		character->AddMoveDir(Vector3(0, -1, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_MOVELEFT]))
	{
		character->AddMoveDir(Vector3(-1, 0, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_MOVERIGHT]))
	{
		character->AddMoveDir(Vector3(1, 0, 0));
	}

	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_SHOOTUP]))
	{
		character->AddShootDir(Vector3(0, 1, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_SHOOTLEFT]))
	{
		character->AddShootDir(Vector3(-1, 0, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_SHOOTDOWN]))
	{
		character->AddShootDir(Vector3(0, -1, 0));
	}
	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_SHOOTRIGHT]))
	{
		character->AddShootDir(Vector3(1, 0, 0));
	}

	if (KeyboardController::GetInstance()->IsKeyDown(key[HOTKEY_SPECIAL]))
	{
		character->UseSpecial();
	}
}

void PlayerInfo::SaveControls(std::string stringKey)
{
	//LuaInterface::GetInstance()->SaveCharValue(fileDirectory, "moveForward", keyMoveForward, true);
	//LuaInterface::GetInstance()->SaveCharValue(fileDirectory, "moveBackward", keyMoveBackward);
	//LuaInterface::GetInstance()->SaveCharValue(fileDirectory, "moveLeft", keyStrafeLeft);
	//LuaInterface::GetInstance()->SaveCharValue(fileDirectory, "moveRight", keyStrafeRight);
	//LuaInterface::GetInstance()->SaveCharValue(fileDirectory, "jump", keyJump);

	//LuaInterface::GetInstance()->SaveFloatValue(fileDirectory, "mouseSensitivity", mouseSensitivity);

	//LuaInterface::GetInstance()->SaveVector3Value(fileDirectory, "playerSpawnPoint", position);
	//LuaInterface::GetInstance()->SaveVector3Value(fileDirectory, "playerSpawnDir", view);
}

void PlayerInfo::LoadControls(std::string stringKey)
{
	key[HOTKEY_MOVEUP] = LuaInterface::GetInstance()->GetCharValue(stringKey, "moveForward");
	key[HOTKEY_MOVEDOWN] = LuaInterface::GetInstance()->GetCharValue(stringKey, "moveBackward");
	key[HOTKEY_MOVELEFT] = LuaInterface::GetInstance()->GetCharValue(stringKey, "moveLeft");
	key[HOTKEY_MOVERIGHT] = LuaInterface::GetInstance()->GetCharValue(stringKey, "moveRight");
	key[HOTKEY_SHOOTUP] = LuaInterface::GetInstance()->GetCharValue(stringKey, "shootForward");
	key[HOTKEY_SHOOTDOWN] = LuaInterface::GetInstance()->GetCharValue(stringKey, "shootBackward");
	key[HOTKEY_SHOOTLEFT] = LuaInterface::GetInstance()->GetCharValue(stringKey, "shootLeft");
	key[HOTKEY_SHOOTRIGHT] = LuaInterface::GetInstance()->GetCharValue(stringKey, "shootRight");
	key[HOTKEY_SPECIAL] = LuaInterface::GetInstance()->GetCharValue(stringKey, "special");
}