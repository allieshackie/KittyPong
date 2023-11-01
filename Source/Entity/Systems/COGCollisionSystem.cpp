#include <algorithm>

#include "Entity/Components/COGBounce.h"
#include "Entity/Components/COGShape.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCircleShape.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/Score.h"

#include "COGCollisionSystem.h"

void COGCollisionSystem::Update(EntityRegistry& entityRegistry) const
{
	const auto view = entityRegistry.GetEnttRegistry().view<COGCollision, COGShape, COGPhysics, COGTransform>();
	view.each([=, &entityRegistry](auto entity, const COGCollision& collider, const COGShape& shape,
	                               COGPhysics& physics, const COGTransform& transform)
	{
		view.each([=, &entityRegistry, &physics](auto otherEntity, COGCollision& otherCollider,
		                                         const COGShape& otherShape, COGPhysics& otherPhysics,
		                                         const COGTransform& otherTransform)
		{
			// do not collide with self
			if (otherEntity != entity)
			{
				if (_CircleBoxCollisionCheck(shape, transform, physics, otherShape, otherTransform))
				{
					collider.OnCollision(physics);

					// other collider has score component, add to score
					auto& score = entityRegistry.GetComponent<Score>(entity);
					score.AddPoint();
				}
			}
		});
	});
}

bool COGCollisionSystem::_CircleBoxCollisionCheck(const COGShape& shape, const COGTransform& transform,
                                                  COGPhysics& physics, const COGShape& otherShape,
                                                  const COGTransform& otherTransform) const
{
	try
	{
		// Attempt to cast the references to derived type
		const auto circle = dynamic_cast<const COGCircleShape&>(shape);
		try
		{
			// Attempt to cast the references to derived type
			const auto box = dynamic_cast<const COGBoxShape&>(otherShape);

			// check a circle colliding with a box (paddle)
			const glm::vec2 closest = {
				std::max(otherTransform.GetPosition().x,
				         std::min(transform.GetPosition().x, otherTransform.GetPosition().x + box.GetWidth())),
				std::max(otherTransform.GetPosition().y,
				         std::min(transform.GetPosition().y, otherTransform.GetPosition().y + box.GetHeight()))
			};

			const glm::vec2 deltaPos = {transform.GetPosition().x - closest.x, transform.GetPosition().y - closest.y};
			// get distance from closest point to ball
			const double distance = (std::pow(deltaPos.x, 2) + std::pow(deltaPos.y, 2)); // get distance

			// if distance is lower than radius
			if (distance - 2 < circle.GetRadius() * 2)
			{
				physics.SetNormal({deltaPos.x / distance, deltaPos.y / distance});
				return true;
			}
		}
		catch (std::bad_cast& e)
		{
			std::cerr << "Dynamic cast failed: " << e.what() << std::endl;
		}
	}
	catch (std::bad_cast& e)
	{
		std::cerr << "Dynamic cast failed: " << e.what() << std::endl;
	}


	return false;
}
