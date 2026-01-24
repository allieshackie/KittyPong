#include <string>
#include "Entity/Components/Score.h"
#include "Entity/Components/COGTransform.h"
#include "Graphics/RenderSystem.h"
#include "Core/World.h"
#include "Entity/Entity.h"

#include "ScoreManager.h"

bool ScoreManager::Update(const RenderSystem& system, std::weak_ptr<World> world, entt::entity player1, entt::entity player2) const
{
	// color {100, 0, 0, 255}

	if (auto worldPtr = world.lock())
	{
		Entity* player1Entity = worldPtr->GetEntityForId(player1);
		auto score1 = player1Entity->GetComponent<Score>();

		Entity* player2Entity = worldPtr->GetEntityForId(player2);
		auto score2 = player2Entity->GetComponent<Score>();

		const std::string sc1 = std::to_string(score1.GetScore());
		const std::string sc2 = std::to_string(score2.GetScore());
		system.DrawTextFont(sc1.c_str(), {350, 45}, {1, 1}, {1,1,1,1}); // display scores  
		system.DrawTextFont(":", {385, 45}, {1, 1}, { 1,1,1,1 });
		system.DrawTextFont(sc2.c_str(), {400, 45}, {1, 1}, { 1,1,1,1 });
		if (score1.GetScore() == 10 || score2.GetScore() == 10)
		{
			// return true if game ends
			score1.SetScore(0);
			score2.SetScore(0);
			return true;
		}
	}

	return false;
}
