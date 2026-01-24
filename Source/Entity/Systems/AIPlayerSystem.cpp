#include "Core/World.h"
#include "Entity/Entity.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"

#include "AIPlayerSystem.h"

AIPlayerSystem::AIPlayerSystem(std::unique_ptr<Player>& aiPlayer, entt::entity ball) : mAIPlayer(aiPlayer),
	mBall(ball)
{
}

void AIPlayerSystem::Update(std::weak_ptr<World>& world) const
{
	if (auto worldPtr = world.lock())
	{
		Entity* ball = worldPtr->GetEntityForId(mBall);
		// get position of the ball and position of the Ai paddle
		const auto& ballPos = ball->GetComponent<COGTransform>().GetPosition();

		Entity* aiPaddle = worldPtr->GetEntityForId(mAIPlayer->GetPaddleId());
		const auto& paddlePos = aiPaddle->GetComponent<COGTransform>().GetPosition();

		auto& paddlePhysics = aiPaddle->GetComponent<COGPhysics>();
		const auto& paddleVelocity = paddlePhysics.GetVelocity();


		// ball is moving right
		if (paddlePos.y < ballPos.y)
		{
			// move up
			paddlePhysics.SetFriction(0.0f);
			paddlePhysics.SetVelocity({0, 75});
		}
		else if (paddlePos.y > ballPos.y)
		{
			// move down
			paddlePhysics.SetFriction(0.0f);
			paddlePhysics.SetVelocity({0, -75});
		}

		else
		{
			paddlePhysics.SetFriction(1.0f);
		}
	}
}
