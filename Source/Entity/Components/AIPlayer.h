#pragma once
#include "Entity/EntityRegistry.h"

class AIPlayer
{
public:
	EntityId GetBallEntity() const { return mBall; }
	void SetBall(const EntityId ball) { mBall = ball; }

private:
	EntityId mBall;
};
