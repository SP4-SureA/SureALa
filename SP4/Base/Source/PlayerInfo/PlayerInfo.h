#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include "SingletonTemplate.h"

class EntityBase;

class PlayerInfo : public Singleton<PlayerInfo>
{
	friend Singleton<PlayerInfo>;
public:
	virtual ~PlayerInfo();

	virtual void UpdateInputs(double dt){}
	virtual void Update(double dt){}

	inline void SetCharacter(EntityBase* _eb){ this->character = _eb; }
	inline EntityBase* GetCharacter(){ return this->character; }

protected:
	PlayerInfo();

	EntityBase* character;
};

#endif