#ifndef _SKY_WHALE_PHASE_1_H
#define Phase1StateManager

#include "../../State/StateManager.h"
#include "SpawnState.h"

class SkyWhalePhase1 : public StateManager
{
public:
	SkyWhalePhase1();
	virtual ~SkyWhalePhase1();

    virtual void Init();
    virtual void Exit();
	virtual void Update(double dt);

protected:
	SpawnState spawnState;

};

#endif // Phase1StateManager