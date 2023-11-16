#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCollision.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Components/Score.h"

#include "Player.h"

#include "Components/COGName.h"
#include "Core/EngineContext.h"


Player::Player(EntityRegistry& registry, glm::vec2 paddlePos, glm::vec3 paddleColor, glm::vec2 boundSize,
               glm::vec2 boundPos, glm::vec3 boundColor)
{
	mPlayer = registry.CreateEntity();
	registry.AddComponent<Score>(mPlayer);

	_CreatePaddle(registry, paddlePos, paddleColor);
	// boundary should be for the opposite side, since a hit will award the player a point
	_CreateBoundary(registry, boundSize, boundPos, boundColor);
}

void Player::SetUserInputs(const EngineContext& context, EntityRegistry& registry, LLGL::Key up, LLGL::Key down) const
{
	context.GetInputHandler().RegisterButtonDownHandler(up, [=, &registry]()
	{
		auto& physics = registry.GetComponent<COGPhysics>(mPaddle);
		physics.SetFriction(1.0f);
		physics.SetVelocity({0.0f, -fMovementSpeed});
	});

	context.GetInputHandler().RegisterButtonUpHandler(up, [=, &registry]()
	{
		auto& physics = registry.GetComponent<COGPhysics>(mPaddle);
		physics.SetFriction(0.4f);
	});

	context.GetInputHandler().RegisterButtonDownHandler(down, [=, &registry]()
	{
		auto& physics = registry.GetComponent<COGPhysics>(mPaddle);
		physics.SetFriction(1.0f);
		physics.SetVelocity({0.0f, fMovementSpeed});
	});

	context.GetInputHandler().RegisterButtonUpHandler(down, [=, &registry]()
	{
		auto& physics = registry.GetComponent<COGPhysics>(mPaddle);
		physics.SetFriction(0.4f);
	});
}

void Player::_CreatePaddle(EntityRegistry& registry, glm::vec2 position, glm::vec3 color)
{
	auto callback = [](COGPhysics& physics, COGPhysics& otherPhysics, int mask)
	{
	};
	int mask = static_cast<int>(CollisionMask::Boundary) | static_cast<int>(CollisionMask::Ball);

	mPaddle = registry.CreateEntity();
	registry.AddComponent<COGTransform>(mPaddle, position);
	registry.AddComponent<COGBoxShape>(mPaddle, fPaddleWidth, fPaddleHeight, color);
	registry.AddComponent<COGPhysics>(mPaddle, glm::vec2{0, 0}, glm::vec2(1, 0));
	registry.AddComponent<COGCollision>(mPaddle, callback, static_cast<int>(CollisionMask::Paddle),
	                                    mask);
	registry.AddComponent<COGName>(mPaddle, "Player Paddle");
}

void Player::_CreateBoundary(EntityRegistry& registry, glm::vec2 size, glm::vec2 position, glm::vec3 color)
{
	auto callback = [this, &registry](COGPhysics& physics, COGPhysics& otherPhysics, int mask)
	{
		auto& score = registry.GetComponent<Score>(mPlayer);
		score.AddPoint();
	};

	mBoundary = registry.CreateEntity();
	registry.AddComponent<COGTransform>(mBoundary, position);
	registry.AddComponent<COGBoxShape>(mBoundary, size.x, size.y, color);
	registry.AddComponent<COGPhysics>(mBoundary, glm::vec2{0, 0}, glm::vec2(1, 0));
	registry.AddComponent<COGCollision>(mBoundary, callback, static_cast<int>(CollisionMask::ScoreBoundary),
	                                    static_cast<int>(CollisionMask::Ball));
	registry.AddComponent<COGName>(mBoundary, "Player Boundary");
}
