#pragma once
#include <glm/vec2.hpp>

class COGPhysics
{
public:
	COGPhysics(glm::vec2 velocity, glm::vec2 normal = {0, 0});

	const glm::vec2& GetVelocity() const { return mVelocity; }
	const glm::vec2& GetNormal() const { return mNormal; }
	float GetFriction() const { return mFriction; }

	void SetNormal(glm::vec2 normal)
	{
		mNormal = normal;
	}

	void SetVelocity(glm::vec2 velocity)
	{
		mVelocity = velocity;
	}

	void SetFriction(float friction)
	{
		mFriction = friction;
	}

private:
	glm::vec2 mVelocity = {0, 0};
	glm::vec2 mNormal = {0, 0};

	float mFriction = 1.0f;
};
