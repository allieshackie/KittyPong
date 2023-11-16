#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"

#include "COGPhysisSystem.h"

void COGPhysicsSystem::Update(float deltaTime, EntityRegistry& entityRegistry) const
{
	const auto view = entityRegistry.GetEnttRegistry().view<COGPhysics, COGTransform>();
	view.each([=](auto& physics, auto& transform)
	{
		// integrate velocity
		auto& pos = transform.GetPosition();
		auto& velocity = physics.GetVelocity();
		float friction = physics.GetFriction();

		glm::vec2 newVelocity = velocity * friction;
		physics.SetVelocity(newVelocity);

		transform.SetPosition({pos.x + newVelocity.x * deltaTime, pos.y + newVelocity.y * deltaTime});
	});
}
