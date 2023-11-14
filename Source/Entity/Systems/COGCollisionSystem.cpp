#include "Entity/Components/COGBounce.h"
#include "Entity/Components/COGShape.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCircleShape.h"
#include "Entity/Components/COGName.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/Score.h"

#include "COGCollisionSystem.h"


void COGCollisionSystem::Update(EntityRegistry& entityRegistry) const
{
	const auto ballView = entityRegistry.GetEnttRegistry().view<
		COGBounce, COGCircleShape, COGPhysics, COGTransform>();
	const auto boxView = entityRegistry.GetEnttRegistry().view<
		COGCollision, COGBoxShape, COGPhysics, COGTransform, COGName>();
	ballView.each([=, &entityRegistry](auto entity, const COGBounce& bounce, const COGCircleShape& ball,
	                                   COGPhysics& physics, const COGTransform& transform)
	{
		boxView.each([=, &entityRegistry, &physics](auto otherEntity, COGCollision& otherCollider,
		                                            const COGBoxShape& box, COGPhysics& otherPhysics,
		                                            const COGTransform& otherTransform, const COGName& name)
		{
			// do not collide with self
			if (otherEntity != entity)
			{
				if (_CircleBoxCollisionCheck(ball, transform, box, otherTransform))
				{
					bounce.OnCollision(physics);

					// other collider has score component, add to score
					//auto& score = entityRegistry.GetComponent<Score>(entity);
					//score.AddPoint();
				}
			}
		});
	});
}

bool COGCollisionSystem::_CircleBoxCollisionCheck(const COGCircleShape& circle, const COGTransform& transform,
                                                  const COGBoxShape& box, const COGTransform& otherTransform) const
{
	// temporary variables to set edges for testing
	float testX = transform.GetPosition().x;
	float testY = transform.GetPosition().y;

	const glm::vec2 topLeftBox = {
		otherTransform.GetPosition().x - (box.GetWidth() / 2), otherTransform.GetPosition().y - (box.GetHeight() / 2)
	};

	// which edge is closest?
	if (transform.GetPosition().x < topLeftBox.x) testX = topLeftBox.x; // test left edge
	else if (transform.GetPosition().x > topLeftBox.x + box.GetWidth()) testX = topLeftBox.x + box.GetWidth();
	// right edge
	if (transform.GetPosition().y < topLeftBox.y) testY = topLeftBox.y; // top edge
	else if (transform.GetPosition().y > topLeftBox.y + box.GetHeight()) testY = topLeftBox.y + box.GetHeight();
	// bottom edge

	// get distance from closest edges
	const float distX = transform.GetPosition().x - testX;
	const float distY = transform.GetPosition().y - testY;
	const float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the radius, collision!
	if (distance <= circle.GetRadius())
	{
		return true;
	}
	return false;
}
