#include "Entity/Components/COGBoxShape.h"
#include "Components/COGBounce.h"
#include "Entity/Components/COGCollision.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Components/Score.h"

#include "Player.h"


Player::Player(EntityRegistry& registry, glm::vec2 paddlePos, glm::vec3 paddleColor, glm::vec2 boundSize,
               glm::vec2 boundPos, glm::vec3 boundColor)
{
	mPlayer = registry.CreateEntity();
	registry.AddComponent<Score>(mPlayer);

	_CreatePaddle(registry, paddlePos, paddleColor);
	// boundary should be for the opposite side, since a hit will award the player a point
	_CreateBoundary(registry, boundSize, boundPos, boundColor);
}

void Player::_CreatePaddle(EntityRegistry& registry, glm::vec2 position, glm::vec3 color)
{
	mPaddle = registry.CreateEntity();
	registry.AddComponent<COGTransform>(mPaddle, position);
	registry.AddComponent<COGBoxShape>(mPaddle, fPaddleWidth, fPaddleHeight, color);
	registry.AddComponent<COGPhysics>(mPaddle, glm::vec2{0, 0});
	registry.AddComponent<COGBounce>(mPaddle);
}

void Player::_CreateBoundary(EntityRegistry& registry, glm::vec2 size, glm::vec2 position, glm::vec3 color)
{
	mBoundary = registry.CreateEntity();
	registry.AddComponent<COGTransform>(mBoundary, position);
	registry.AddComponent<COGBoxShape>(mBoundary, size.x, size.y, color);
	registry.AddComponent<COGPhysics>(mBoundary, glm::vec2{0, 0});
	registry.AddComponent<COGCollision>(mBoundary);

	auto collision = registry.GetComponent<COGCollision>(mBoundary);
	collision.SetCollisionCallback([this, &registry](COGPhysics& physics)
	{
		auto score = registry.GetComponent<Score>(mPlayer);
		score.AddPoint();
	});
}
