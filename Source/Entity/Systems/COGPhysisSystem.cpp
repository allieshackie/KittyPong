#include "Core/World.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"

#include "COGPhysisSystem.h"

COGPhysicsSystem::COGPhysicsSystem(std::weak_ptr<World> world) : mWorld(world)
{
}

void COGPhysicsSystem::Update(float dt)
{
	if (auto worldPtr = mWorld.lock())
	{
		const auto view = worldPtr->GetRegistry().view<COGPhysics, COGTransform>();
		view.each([=](auto& physics, auto& transform)
		{
			// integrate velocity
			auto& pos = transform.GetPosition();
			auto& velocity = physics.GetVelocity();
			float friction = physics.GetFriction();

			glm::vec2 newVelocity = velocity * friction;
			physics.SetVelocity(newVelocity);

			transform.SetPosition({pos.x + newVelocity.x * dt, pos.y + newVelocity.y * dt });
		});
	}
}
