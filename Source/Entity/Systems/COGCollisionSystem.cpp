#include "Core/World.h"
#include "Entity/Components/COGBounce.h"
#include "Entity/Components/COGShape.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCircleShape.h"
#include "Entity/Components/COGPhysics.h"

#include "COGCollisionSystem.h"

COGCollisionSystem::COGCollisionSystem(std::weak_ptr<World> world) : mWorld(world)
{
}

void COGCollisionSystem::Update(float dt)
{
	if (auto worldPtr = mWorld.lock())
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
		boxView.each([=](auto entity, const COGCollision& collider, const COGBoxShape& box, COGPhysics& physics, const COGTransform& transform)
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
	const glm::vec2 circlePos = transform.GetPosition();
	const glm::vec2 boxPos = otherTransform.GetPosition();

	// Calculate box bounds (assuming position is the CENTER of the box)
	float boxLeft = boxPos.x - box.GetWidth();
	float boxRight = boxPos.x + box.GetWidth();
	float boxTop = boxPos.y + box.GetHeight();
	float boxBottom = boxPos.y - box.GetHeight();

	// Find the closest point on the box to the circle
	float testX = circlePos.x;
	float testY = circlePos.y;

	if (circlePos.x < boxLeft) testX = boxLeft;
	else if (circlePos.x > boxRight) testX = boxRight;

	if (circlePos.y < boxBottom) testY = boxBottom;
	else if (circlePos.y > boxTop) testY = boxTop;

	// Calculate distance from circle center to closest point
	float distX = circlePos.x - testX;
	float distY = circlePos.y - testY;
	float distance = sqrt((distX * distX) + (distY * distY));

	return distance <= circle.GetRadius();
}

bool COGCollisionSystem::_BoxBoxCollisionCheck(const COGBoxShape& box, const COGTransform& transform,
                                               const COGBoxShape& otherBox, const COGTransform& otherTransform) const
{
	const glm::vec2 boxPos = transform.GetPosition();
	const glm::vec2 otherBoxPos = otherTransform.GetPosition();

	// Calculate box bounds (assuming position is the CENTER of each box)
	float box1Left = boxPos.x - box.GetWidth();
	float box1Right = boxPos.x + box.GetWidth();
	float box1Top = boxPos.y + box.GetHeight();
	float box1Bottom = boxPos.y - box.GetHeight();

	float box2Left = otherBoxPos.x - otherBox.GetWidth();
	float box2Right = otherBoxPos.x + otherBox.GetWidth();
	float box2Top = otherBoxPos.y + otherBox.GetHeight();
	float box2Bottom = otherBoxPos.y - otherBox.GetHeight();

	// AABB collision check
	return (box1Right > box2Left &&
		box1Left < box2Right &&
		box1Top > box2Bottom &&
		box1Bottom < box2Top);
}
