#ifndef _SKY_WHALE_PHASE_2_H
#define Phase1StateManager

#include "../../State/StateManager.h"
#include "../Phase2/AngryState.h"

class SkyWhalePhase2 : public StateManager
{
public:
	SkyWhalePhase2();
	virtual ~SkyWhalePhase2();

    virtual void Init();
	virtual void Update(double dt);

protected:
    AngryState angryState;
};

#endif // Phase1StateManager