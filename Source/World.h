#pragma once
#include "Entity/Player.h"
#include "Entity/Systems/ScoreManager.h"

class EntityRegistry;
class GameObject;

class World
{
public:
	World(const EngineContext& engine, EntityRegistry& registry);

	void Update(const EngineContext& engine, EntityRegistry& registry) const;
	void Render(const EngineContext& engine, EntityRegistry& registry) const;

	void SetPlayerAI(EntityRegistry& registry) const;

	bool HasGameStarted() const { return mHasGameStarted; }

private:
	void _Menu(const EngineContext& engine) const;
	void _UpdateCatFeatures(const EngineContext& engine);
	void _InitGameplayEntities(const EngineContext& engine, EntityRegistry& registry);

	EntityId _CreateBall(EntityRegistry& registry, glm::vec2 position, glm::vec3 color) const;

	// Render Helpers
	void _DrawKitty(const EngineContext& engine) const;
	void _DrawEyes(const EngineContext& engine) const;
	void _DrawMouth(const EngineContext& engine) const;

	// User Inputs
	void _ChooseButton1();
	void _ChooseButton2();
	void _ChooseButtonEnter();

	// user input and score manager 
	std::unique_ptr<ScoreManager> mScoreManager;
	std::unique_ptr<Player> mPlayer1;
	std::unique_ptr<Player> mPlayer2;
	EntityId mBall;

	bool mInput1 = false;
	bool mInput2 = false;

	int mScoreAnimationTimer = 0;

	bool mHasGameStarted = false;

	const float fBallRadius = 10.0f;
	glm::vec2 mEyePos1 = {-40.0f, 250};
	glm::vec2 mEyePos2 = {40.0f, 250};
};
