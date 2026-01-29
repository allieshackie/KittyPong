#include "Core/World.h"
#include "Entity/Entity.h"
#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCollision.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Components/Score.h"
#include "Entity/Components/COGName.h"
#include "Input/InputHandler.h"

#include "Player.h"

Player::Player(std::weak_ptr<World> world, glm::vec2 paddlePos, glm::vec4 paddleColor, glm::vec2 boundSize,
               glm::vec2 boundPos, glm::vec4 boundColor) : mWorld(world)
{
	if (auto worldPtr = world.lock()) 
	{
		Entity& player = worldPtr->CreateEntity();
		player.AddComponentWithArgs<Score>();

		mPlayer = player.GetId();
	}

	_CreatePaddle(paddlePos, paddleColor);
	// boundary should be for the opposite side, since a hit will award the player a point
	_CreateBoundary(boundSize, boundPos, boundColor);
}

void Player::SetUserInputs(std::weak_ptr<InputHandler> inputHandler, LLGL::Key up, LLGL::Key down) const
{
	if (auto inputPtr = inputHandler.lock())
	{
		inputPtr->RegisterButtonHoldHandler(up, 
			[=](){
				if (auto worldPtr = mWorld.lock())
				{
					Entity* paddle = worldPtr->GetEntityForId(mPaddle);
					auto& physics = paddle->GetComponent<COGPhysics>();
					physics.SetFriction(1.0f);
					physics.SetVelocity({0.0f, fMovementSpeed});
				}
			}, 
			[=](){
				if (auto worldPtr = mWorld.lock())
				{
					Entity* paddle = worldPtr->GetEntityForId(mPaddle);
					auto& physics = paddle->GetComponent<COGPhysics>();
					physics.SetFriction(0.4f);
				}
			}
		);

		inputPtr->RegisterButtonHoldHandler(down, 
			[=]()
			{
				if (auto worldPtr = mWorld.lock())
				{
					Entity* paddle = worldPtr->GetEntityForId(mPaddle);
					auto& physics = paddle->GetComponent<COGPhysics>();
					physics.SetFriction(1.0f); 
					physics.SetVelocity({0.0f, -fMovementSpeed});
				}
			}, 
			[=]()
			{
				if (auto worldPtr = mWorld.lock())
				{
					Entity* paddle = worldPtr->GetEntityForId(mPaddle);
					auto& physics = paddle->GetComponent<COGPhysics>();
					physics.SetFriction(0.4f);
				}
			}
		);
	}
}

void Player::_CreatePaddle(glm::vec2 position, glm::vec4 color)
{
	auto callback = [](COGPhysics& physics, COGPhysics& otherPhysics, int mask)
	{
	};
	int mask = static_cast<int>(CollisionMask::Boundary) | static_cast<int>(CollisionMask::Ball);

	if (auto worldPtr = mWorld.lock())
	{
		Entity& paddle = worldPtr->CreateEntity();
		paddle.AddComponentWithArgs<COGTransform>(position);
		paddle.AddComponentWithArgs<COGBoxShape>(fPaddleWidth, fPaddleHeight, color);
		paddle.AddComponentWithArgs<COGPhysics>(glm::vec2{0, 0}, glm::vec2(1, 0));
		paddle.AddComponentWithArgs<COGCollision>(callback, static_cast<int>(CollisionMask::Paddle),
											mask);
		paddle.AddComponentWithArgs<COGName>("Player Paddle");
	
		mPaddle = paddle.GetId();
	}
}

void Player::_CreateBoundary(glm::vec2 size, glm::vec2 position, glm::vec4 color)
{
	auto callback = [this](COGPhysics& physics, COGPhysics& otherPhysics, int mask)
	{
		if (auto worldPtr = mWorld.lock())
		{
			Entity* player = worldPtr->GetEntityForId(mPlayer);
			auto& score = player->GetComponent<Score>();
			score.AddPoint();
		}
	};

	if (auto worldPtr = mWorld.lock())
	{
		Entity& boundary = worldPtr->CreateEntity();
		boundary.AddComponentWithArgs<COGTransform>(position);
		boundary.AddComponentWithArgs<COGBoxShape>(size.x, size.y, color);
		boundary.AddComponentWithArgs<COGPhysics>(glm::vec2{0, 0}, glm::vec2(1, 0));
		boundary.AddComponentWithArgs<COGCollision>(callback, static_cast<int>(CollisionMask::ScoreBoundary),
											static_cast<int>(CollisionMask::Ball));
		boundary.AddComponentWithArgs<COGName>("Player Boundary");

		mBoundary = boundary.GetId();
	}
}
