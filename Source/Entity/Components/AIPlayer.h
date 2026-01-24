#pragma once
#include <entt/entt.hpp>

class AIPlayer
{
public:
	entt::entity GetBallEntity() const { return mBall; }
	void SetBall(const entt::entity ball) { mBall = ball; }

private:
	entt::entity mBall;
};
