#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Entity/EntityRegistry.h"
#include "Entity/Player.h"
#include "Entity/Components/AIPlayer.h"
#include "Entity/Components/COGBounce.h"
#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCircleShape.h"
#include "Entity/Components/COGCollision.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Systems/ScoreManager.h"

#include "World.h"

void World::Update(const EngineContext& engine, EntityRegistry& registry) const
{
	// continually update score
	mScoreManager->Update(engine, registry, mPlayer1->GetPlayerId(), mPlayer2->GetPlayerId());
}

void World::SetPlayerAI(EntityRegistry& registry) const
{
	const auto id = mPlayer2->GetPlayerId();
	registry.AddComponent<AIPlayer>(id);
}

void World::Init(EntityRegistry& registry)
{
	mPlayer1 = std::make_unique<Player>(registry, glm::vec2{0, 100}, glm::vec3{255, 229, 128}, glm::vec2{1, 600},
	                                    glm::vec2{799, 0}, glm::vec3{0, 0, 255});

	mPlayer2 = std::make_unique<Player>(registry, glm::vec2{780, 100}, glm::vec3{255, 229, 128}, glm::vec2{1, 600},
	                                    glm::vec2{0, 0}, glm::vec3{0, 0, 255});
	// boundaries
	const auto boundary1 = registry.CreateEntity();
	registry.AddComponent<COGTransform>(boundary1, glm::vec2{800, 1});
	registry.AddComponent<COGBoxShape>(boundary1, 0, 599, glm::vec3{0, 0, 255});
	registry.AddComponent<COGPhysics>(boundary1, glm::vec2{0, 0});
	registry.AddComponent<COGCollision>(boundary1);

	const auto boundary2 = registry.CreateEntity();
	registry.AddComponent<COGTransform>(boundary2, glm::vec2{800, 1});
	registry.AddComponent<COGBoxShape>(boundary2, 0, 0, glm::vec3{0, 0, 255});
	registry.AddComponent<COGPhysics>(boundary2, glm::vec2{0, 0});
	registry.AddComponent<COGCollision>(boundary2);

	auto ball = _CreateBall(registry, {200, 200}, {179, 170, 154});
	mScoreManager = std::make_unique<ScoreManager>(ball);
}


EntityId World::_CreateBall(EntityRegistry& registry, glm::vec2 position, glm::vec3 color) const
{
	const auto ballEntity = registry.CreateEntity();
	registry.AddComponent<COGTransform>(ballEntity, position);
	registry.AddComponent<COGCircleShape>(ballEntity, fBallRadius, color);
	registry.AddComponent<COGPhysics>(ballEntity, glm::vec2{250, 150});
	registry.AddComponent<COGBounce>(ballEntity);

	return ballEntity;
}
