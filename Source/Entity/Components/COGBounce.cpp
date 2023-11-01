#include "Entity/Components/COGPhysics.h"

#include "COGBounce.h"

COGBounce::COGBounce()
{
	SetCollisionCallback([](COGPhysics& physics)
	{
		const auto& normal = physics.GetNormal();
		const auto& velocity = physics.GetVelocity();
		// reflect on x if normal is opposite of velocity
		if (normal.x > 0 && velocity.x < 0 || normal.x < 0 && velocity.x > 0)
		{
			physics.SetVelocity({-velocity.x, velocity.y});
		}
		// reflect on y if normal is opposite of velocity
		if (normal.y > 0 && velocity.y < 0 || normal.y < 0 && velocity.y > 0)
		{
			physics.SetVelocity({velocity.x, -velocity.y});
		}
	});
}
