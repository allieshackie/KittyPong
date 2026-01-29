#pragma once

#include "Core/ISystem.h"

class World;

class COGPhysicsSystem : public ISystem
{
public:
	COGPhysicsSystem(std::weak_ptr<World> world);

	void Update(float dt) override;

private:
	std::weak_ptr<World> mWorld;
};
