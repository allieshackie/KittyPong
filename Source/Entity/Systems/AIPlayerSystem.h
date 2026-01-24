#pragma once
#include <entt/entt.hpp>
#include "Entity/Player.h"

class World;

class AIPlayerSystem
{
public:
	AIPlayerSystem(std::unique_ptr<Player>& aiPlayer, entt::entity ball);

	void Update(std::weak_ptr<World>& world) const;

private:
	std::unique_ptr<Player>& mAIPlayer;
	entt::entity mBall;
};
