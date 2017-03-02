#ifndef GAME_INFO_H
#define GAME_HINFO_H

#include "Vector3.h"
#include "SingletonTemplate.h"
#include "..\PlayerInfo\PlayerInfo.h"

#include <vector>
using std::vector;

class PlayerEntityBase;
class ScrollingCamera;

class GameInfo : public Singleton<GameInfo>
{
	friend Singleton<GameInfo>;
public:
	enum GAME_MODE
	{
		GAMEMODE_TUTORIAL,
		GAMEMODE_STORY,
		GAMEMODE_ENDLESS,
		
		GAMEMODE_END
	};
	GameInfo();
	~GameInfo();

	void Init();

	inline std::vector<PlayerInfo*> GetPlayersList(){ return this->playersList; }
	
	inline void AddPlayer(PlayerInfo* player){ if (player == NULL){ return; } playersList.push_back(player); }
	bool RemovePlayer(PlayerInfo* player);

	PlayerEntityBase* GetNearestPlayer(Vector3 fromPos);
	PlayerEntityBase* GetRandomPlayer();

	inline int GetHealth(){ return this->health; }
	inline void SetHealth(unsigned int hp){ this->health = hp; }
	inline void TakeDamage(int amount){ this->health -= amount; if (health < 0){ health = 0; } }

	inline unsigned int GetScore(){ return this->score; }
	inline void SetScore(unsigned int _score){ this->score = _score; }

	inline void SetGameMode(GAME_MODE gm){ this->gameMode = gm; }
	inline GAME_MODE GetGameMode(){ return this->gameMode; }

protected:
	GAME_MODE gameMode;
	int health;
	unsigned int score;
	std::vector<PlayerInfo*> playersList;
};

#endif