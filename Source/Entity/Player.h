#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "LLGL/Key.h"

class InputHandler;
class World;

class Player
{
public:
	Player(std::weak_ptr<World> world, glm::vec2 paddlePos, glm::vec4 paddleColor, glm::vec2 boundSize,
	       glm::vec2 boundPos, glm::vec4 boundColor);

	entt::entity GetPlayerId() const { return mPlayer; }
	entt::entity GetPaddleId() const { return mPaddle; }

	void SetUserInputs(std::weak_ptr<InputHandler> inputHandler, LLGL::Key up, LLGL::Key down) const;

private:
	void _CreatePaddle(glm::vec2 position, glm::vec4 color);
	void _CreateBoundary(glm::vec2 size, glm::vec2 position,
	                     glm::vec4 color);

	entt::entity mPlayer;
	entt::entity mPaddle;
	entt::entity mBoundary;

	std::weak_ptr<World> mWorld;

	const float fPaddleWidth = 0.5f;
	const float fPaddleHeight = 1.5f;

	const float fMovementSpeed = 2.0f;
};
