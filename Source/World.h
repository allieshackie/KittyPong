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
	void Render(const EngineContext& engine);

	void SetPlayerAI(EntityRegistry& registry) const;

	bool HasGameStarted() const { return mHasGameStarted; }

private:
	void _Menu(const EngineContext& engine) const;

	EntityId _CreateBall(EntityRegistry& registry, glm::vec2 position, glm::vec3 color) const;

	void _DrawKitty(const EngineContext& engine);
	void _DrawEyes(const EngineContext& engine);
	void _DrawMouth(const EngineContext& engine);

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

	const float fBallRadius = 12.0f;
	glm::vec2 mEyePos1 = {370, 282};
	glm::vec2 mEyePos2 = {428, 282};
};
