#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "LLGL/Key.h"

#include "Entity/EntityRegistry.h"

class EngineContext;

class Player
{
public:
	Player(EntityRegistry& registry, glm::vec2 paddlePos, glm::vec4 paddleColor, glm::vec2 boundSize,
	       glm::vec2 boundPos, glm::vec4 boundColor);

	EntityId GetPlayerId() const { return mPlayer; }
	EntityId GetPaddleId() const { return mPaddle; }

	void SetUserInputs(const EngineContext& context, EntityRegistry& registry, LLGL::Key up, LLGL::Key down) const;

private:
	void _CreatePaddle(EntityRegistry& registry, glm::vec2 position, glm::vec4 color);
	void _CreateBoundary(EntityRegistry& registry, glm::vec2 size, glm::vec2 position,
	                     glm::vec4 color);

	EntityId mPlayer;
	EntityId mPaddle;
	EntityId mBoundary;

	const float fPaddleWidth = 20.0f;
	const float fPaddleHeight = 100.0f;

	const float fMovementSpeed = 100.0f;
};
