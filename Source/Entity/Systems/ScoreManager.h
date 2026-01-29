#pragma once
#include <entt/entt.hpp>

class RenderSystem;
class World;

class ScoreManager
{
public:
	ScoreManager(const RenderSystem& system);

	bool Update(RenderSystem& system, std::weak_ptr<World> world, entt::entity player1,
	            entt::entity player2);

private:
	uint32_t mScoreText;

	int mScore1 = -1;
	int mScore2 = -1;
};
