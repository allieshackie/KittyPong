#include "Core/World.h"
#include "Entity/Components/COGBounce.h"
#include "Entity/Components/COGShape.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCircleShape.h"
#include "Entity/Components/COGPhysics.h"

#include "COGCollisionSystem.h"

void COGCollisionSystem::Update(std::weak_ptr<World> world) const
{
	if (auto worldPtr = world.lock())
	{
		const auto ballView = worldPtr->GetRegistry().view<COGBounce, COGCircleShape, COGPhysics, COGTransform>();
		const auto boxView = worldPtr->GetRegistry().view<COGCollision, COGBoxShape, COGPhysics, COGTransform>();

		// Circle - box interactions
		ballView.each([=](const COGBounce& bounce, const COGCircleShape& ball,
										   COGPhysics& physics, const COGTransform& transform)
		{
			boxView.each([=, &physics, &bounce](const COGCollision& otherCollider,
																 const COGBoxShape& box, COGPhysics& otherPhysics,
																 const COGTransform& otherTransform)
			{
				if (_CanCollide(bounce, otherCollider))
				{
					if (_CircleBoxCollisionCheck(ball, transform, box, otherTransform))
					{
						bounce.OnCollision(physics, otherPhysics, otherCollider.GetIdentity());
						otherCollider.OnCollision(physics, otherPhysics, bounce.GetIdentity());
					}
				}
			});
		});

		// Box - Box interactions
		boxView.each([=](auto entity, const COGCollision& collider,
										  const COGBoxShape& box, COGPhysics& physics,
										  const COGTransform& transform)
		{
			boxView.each([=, &physics, &collider](auto otherEntity, const COGCollision& otherCollider,
																   const COGBoxShape& otherBox, COGPhysics& otherPhysics,
																   const COGTransform& otherTransform)
			{
				// Don't compare against self
				if (entity != otherEntity)
				{
					if (_CanCollide(collider, otherCollider))
					{
						if (_BoxBoxCollisionCheck(box, transform, otherBox, otherTransform))
						{
							collider.OnCollision(physics, otherPhysics, otherCollider.GetIdentity());
							otherCollider.OnCollision(physics, otherPhysics, collider.GetIdentity());
						}
					}
				}
			});
		});
	}
}

bool COGCollisionSystem::_CanCollide(const COGCollision& collision, const COGCollision& otherCollision) const
{
	if ((collision.GetIdentity() & otherCollision.GetMask()) == 0)
	{
		return false;
	}

	if ((otherCollision.GetIdentity() & collision.GetMask()) == 0)
	{
		return false;
	}

	return true;
}

bool COGCollisionSystem::_CircleBoxCollisionCheck(const COGCircleShape& circle, const COGTransform& transform,
                                                  const COGBoxShape& box, const COGTransform& otherTransform) const
{
	// temporary variables to set edges for testing
	float testX = transform.GetPosition().x;
	float testY = transform.GetPosition().y;

	const glm::vec2 topLeftBox = otherTransform.GetPosition();

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

bool COGCollisionSystem::_BoxBoxCollisionCheck(const COGBoxShape& box, const COGTransform& transform,
                                               const COGBoxShape& otherBox, const COGTransform& otherTransform) const
{
	auto& boxPos = transform.GetPosition();
	auto& otherBoxPos = otherTransform.GetPosition();

	if (boxPos.x < otherBoxPos.x + otherBox.GetWidth() && // is box to left of otherBox?
		boxPos.x + box.GetWidth() > otherBoxPos.x && // is box to right of otherBox?
		boxPos.y < otherBoxPos.y + otherBox.GetHeight() &&
		boxPos.y + box.GetHeight() > otherBoxPos.y)
	{
		return true;
	}

	return false;
}
