#pragma once
#include "Entity/EntityRegistry.h"

class COGPhysicsSystem
{
public:
	void Update(float deltaTime, EntityRegistry& entityRegistry) const;
};
