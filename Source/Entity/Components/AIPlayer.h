#pragma once
#include "Entity/EntityRegistry.h"

class AIPlayer
{
public:
	void SetBall(const EntityId ball) { mBall = ball; }
private:
	EntityId mBall;
};

/*
 *
void AIPlayer::Update(float fDeltaT)
{
	// get position of the ball and position of the Ai paddle
	auto transform = mBall->FindComponent<COGTransform>(ComponentType::Transform);
	const exVector2& pos = transform->GetPosition();
	exVector2& paddlePos = mGO->FindComponent<COGTransform>(ComponentType::Transform)->GetPosition();
	if (pos.x > 0 && pos.x > 350)
	{
		// ball is moving right
		if (paddlePos.y < pos.y - 30)
		{
			// move up
			paddlePos.y += 15; // intentionally messing up
			paddlePos.y -= 5;
		}
		else if (paddlePos.y > pos.y + 30)
		{
			// move down
			paddlePos.y -= 15;
			paddlePos.y += 7;
		}
	}
}
 */
