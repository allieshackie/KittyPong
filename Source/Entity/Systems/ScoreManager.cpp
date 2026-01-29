#include <string>
#include "Entity/Components/Score.h"
#include "Entity/Components/COGTransform.h"
#include "Graphics/RenderSystem.h"
#include "Core/World.h"
#include "Entity/Entity.h"

#include "ScoreManager.h"

ScoreManager::ScoreManager(const RenderSystem& system)
{
	mScoreText = system.AddText("", { -0.4, 2.8 }, { 0.01, -0.01 }, { 100, 0, 0, 255 });
}

bool ScoreManager::Update(RenderSystem& system, std::weak_ptr<World> world, entt::entity player1, entt::entity player2)
{
	// color {100, 0, 0, 255}

	if (auto worldPtr = world.lock())
	{
		Entity* player1Entity = worldPtr->GetEntityForId(player1);
		auto score1 = player1Entity->GetComponent<Score>();

		Entity* player2Entity = worldPtr->GetEntityForId(player2);
		auto score2 = player2Entity->GetComponent<Score>();

		if (mScore1 != score1.GetScore() || mScore2 != score2.GetScore())
		{
			const std::string sc1 = std::to_string(score1.GetScore());
			const std::string sc2 = std::to_string(score2.GetScore());
			system.UpdateText(mScoreText, sc1 + ":" + sc2);

			mScore1 = score1.GetScore();
			mScore2 = score2.GetScore();
		}

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
