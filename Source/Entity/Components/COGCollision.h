#pragma once

#include <functional>

#include "COGPhysics.h"

class COGPhysics;

enum class CollisionMask : int
{
	All = ~0,
	Boundary = 1 << 0,
	Paddle = 1 << 1,
	Ball = 1 << 2,
	ScoreBoundary = 1 << 3,
};

class COGCollision
{
public:
	COGCollision(std::function<void(COGPhysics& physics, COGPhysics& otherPhysics, int mask)> callback,
	             int identity = static_cast<int>(CollisionMask::All),
	             int mask = static_cast<int>(CollisionMask::All)) : mCollisionCallback(std::move(callback)),
	                                                                mIdentity(identity), mMask(mask)
	{
	}

	int GetIdentity() const { return mIdentity; }
	int GetMask() const { return mMask; }

	void OnCollision(COGPhysics& physics, COGPhysics& otherPhysics, int mask) const
	{
		if (mCollisionCallback)
		{
			mCollisionCallback(physics, otherPhysics, mask);
		}
	}

private:
	std::function<void(COGPhysics& physics, COGPhysics& otherPhysics, int mask)> mCollisionCallback;

	int mIdentity = static_cast<int>(CollisionMask::All);
	int mMask = static_cast<int>(CollisionMask::All);
};
