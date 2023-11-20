#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"

#include "AIPlayerSystem.h"

AIPlayerSystem::AIPlayerSystem(std::unique_ptr<Player>& aiPlayer, EntityId ball) : mAIPlayer(aiPlayer),
	mBall(ball)
{
}

void AIPlayerSystem::Update(EntityRegistry& entityRegistry) const
{
	// get position of the ball and position of the Ai paddle
	const auto& ballPos = entityRegistry.GetComponent<COGTransform>(mBall).GetPosition();
	const auto& paddlePos = entityRegistry.GetComponent<COGTransform>(mAIPlayer->GetPaddleId()).GetPosition();

	auto& paddlePhysics = entityRegistry.GetComponent<COGPhysics>(mAIPlayer->GetPaddleId());
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
