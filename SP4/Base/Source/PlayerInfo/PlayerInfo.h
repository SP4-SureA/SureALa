#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include "SingletonTemplate.h"

class PlayerEntityBase;

class PlayerInfo : public Singleton<PlayerInfo>
{
	friend Singleton<PlayerInfo>;
public:
	virtual ~PlayerInfo();

	virtual void UpdateInputs(double dt){}
	virtual void Update(double dt){}

	inline void SetCharacter(PlayerEntityBase* _eb){ this->character = _eb; }
	inline PlayerEntityBase* GetCharacter(){ return this->character; }

protected:
	PlayerInfo();

	PlayerEntityBase* character;
};

#endif