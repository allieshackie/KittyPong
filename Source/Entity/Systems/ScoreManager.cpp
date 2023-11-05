#include <string>
#include "Core/EngineContext.h"
#include "Entity/EntityRegistry.h"
#include "Entity/Components/Score.h"
#include "Entity/Components/COGTransform.h"

#include "ScoreManager.h"

bool ScoreManager::Update(const EngineContext& engine, EntityRegistry& entityRegistry, const EntityId& player1,
                          const EntityId& player2) const
{
	// color {100, 0, 0, 255}
	engine.DrawText2D("KITTY PONG", {300, 10}, {1, 1}); // display header
	auto score1 = entityRegistry.GetComponent<Score>(player1);
	auto score2 = entityRegistry.GetComponent<Score>(player2);
	const std::string sc1 = std::to_string(score1.GetScore());
	const std::string sc2 = std::to_string(score2.GetScore());
	engine.DrawText2D(sc1.c_str(), {350, 45}, {1, 1}); // display scores  
	engine.DrawText2D(":", {385, 45}, {1, 1});
	engine.DrawText2D(sc2.c_str(), {400, 45}, {1, 1});
	if (score1.GetScore() == 10 || score2.GetScore() == 10)
	{
		// return true if game ends
		score1.SetScore(0);
		score2.SetScore(0);
		return true;
	}

	return false;
}
