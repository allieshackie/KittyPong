#include "Entity/Components/COGPhysics.h"

#include "COGBounce.h"

COGBounce::COGBounce() : COGCollision(
	[](COGPhysics& physics, COGPhysics& otherPhysics, int mask)
	{
		const float dotProduct = physics.GetVelocity().x * otherPhysics.GetNormal().x + physics.GetVelocity().y *
			otherPhysics
			.GetNormal().y;

		// Reflect the velocity using the formula: v' = v - 2 * (v dot n) * n
		physics.SetVelocity({
			physics.GetVelocity().x - 2.0f * dotProduct * otherPhysics.GetNormal().x,
			physics.GetVelocity().y - 2.0f * dotProduct * otherPhysics.GetNormal().y
		});
	}, static_cast<int>(CollisionMask::Ball), static_cast<int>(CollisionMask::All))
{
}
