#pragma once
#include "Entity/Player.h"

class EntityRegistry;

class AIPlayerSystem
{
public:
	AIPlayerSystem(std::unique_ptr<Player>& aiPlayer, EntityId ball);

	void Update(EntityRegistry& entityRegistry) const;

private:
	std::unique_ptr<Player>& mAIPlayer;
	EntityId mBall;
};
