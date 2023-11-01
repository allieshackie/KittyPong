#pragma once

#include <functional>
#include "Entity/EntityRegistry.h"

class COGPhysics;

class COGCollision
{
public:
	void OnCollision(COGPhysics& physics) const
	{
		if (mCollisionCallback)
		{
			mCollisionCallback(physics);
		}
	}

	void SetCollisionCallback(std::function<void(COGPhysics& physics)> callback)
	{
		mCollisionCallback = std::move(callback);
	}

private:
	std::function<void(COGPhysics& physics)> mCollisionCallback;
};
