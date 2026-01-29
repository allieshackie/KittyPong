#pragma once
#include "Entity/Entity.h"
#include "Entity/Player.h"
#include "Entity/Systems/AIPlayerSystem.h"
#include "Entity/Systems/ScoreManager.h"

class InputHandler;
class World;

class PongWorld
{
public:
	PongWorld(std::weak_ptr<InputHandler> inputHandler, std::weak_ptr<World> world, RenderSystem& renderSystem);

	void Update();
	void Render();

	void SetPlayerAI() const;

	bool HasGameStarted() const { return mHasGameStarted; }

private:
	void _Menu();
	void _UpdateCatFeatures();
	void _InitGameplayEntities(std::weak_ptr<InputHandler> inputHandler);

	void _CreateBall(glm::vec2 position, glm::vec4 color);

	// Render Helpers
	void _DrawKitty() const;

	// User Inputs
	void _ChooseButton1();
	void _ChooseButton2();
	void _ChooseButtonEnter();

	// user input and score manager 
	std::unique_ptr<ScoreManager> mScoreManager;
	std::unique_ptr<AIPlayerSystem> mAIPlayerSystem;
	std::unique_ptr<Player> mPlayer1;
	std::unique_ptr<Player> mPlayer2;
	entt::entity mBall = entt::entity{};

	uint32_t mHeaderText = 0;
	uint32_t mButton1 = 0;
	uint32_t mButton2 = 0;
	uint32_t mActionText = 0;

	bool mInitializeText = false;

	std::weak_ptr<World> mWorld;
	RenderSystem& mRenderSystem;

	bool mInput1 = false;
	bool mInput2 = false;

	int mScoreAnimationTimer = 0;

	bool mHasGameStarted = false;

	const float fBallRadius = 0.2f;
	glm::vec2 mEyePos1 = { -0.7, 0.2 };
	glm::vec2 mEyePos2 = { 0.7, 0.2 };
};
