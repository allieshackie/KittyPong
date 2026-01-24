#pragma once

class World;

class COGPhysicsSystem
{
public:
	void Update(float deltaTime, std::weak_ptr<World> world) const;
};
