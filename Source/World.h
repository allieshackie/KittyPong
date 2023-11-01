#pragma once

class EntityRegistry;
class GameObject;
class Player;
class ScoreManager;

class World
{
public:
	void Init(EntityRegistry& registry);
	void Update(const EngineContext& engine, EntityRegistry& registry) const;

	void SetPlayerAI(EntityRegistry& registry) const;

private:
	EntityId _CreateBall(EntityRegistry& registry, glm::vec2 position, glm::vec3 color) const;

	// user input and score manager 
	std::unique_ptr<ScoreManager> mScoreManager;
	std::unique_ptr<Player> mPlayer1;
	std::unique_ptr<Player> mPlayer2;

	const float fBallRadius = 12.0f;
};
