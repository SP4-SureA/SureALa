#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <string>
using std::string;

class PlayerEntityBase;

class PlayerInfo
{
public:
	enum HOTKEYS
	{
		HOTKEY_MOVEUP,
		HOTKEY_MOVEDOWN,
		HOTKEY_MOVELEFT,
		HOTKEY_MOVERIGHT,
		HOTKEY_SHOOTUP,
		HOTKEY_SHOOTDOWN,
		HOTKEY_SHOOTLEFT,
		HOTKEY_SHOOTRIGHT,
		HOTKEY_SPECIAL,

		HOTKEYS_END
	};
	PlayerInfo();
	virtual ~PlayerInfo();

	virtual void UpdateInputs(double dt);
	virtual void Update(double dt){}

	inline void SetCharacter(PlayerEntityBase* _eb){ this->character = _eb; }
	inline PlayerEntityBase* GetCharacter(){ return this->character; }

	inline unsigned char GetKey(HOTKEYS hotkey){ return this->key[hotkey]; }

	// Handling Lua
	void SaveControls(std::string stringKey);
	void LoadControls(std::string stringKey);

protected:
	PlayerEntityBase* character;

	unsigned char key[HOTKEYS_END];
};

#endif