#pragma once
#include <entt/entt.hpp>

class RenderSystem;
class World;

class ScoreManager
{
public:
	bool Update(const RenderSystem& system, std::weak_ptr<World> world, entt::entity player1,
	            entt::entity player2) const;
};
