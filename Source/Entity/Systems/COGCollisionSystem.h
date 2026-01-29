#pragma once

#include "Core/ISystem.h"

class COGCollision;
class COGBoxShape;
class COGCircleShape;
class COGPhysics;
class COGTransform;
class World;

class COGCollisionSystem : public ISystem
{
public:
	COGCollisionSystem(std::weak_ptr<World> world);

	void Update(float dt) override;

private:
	bool _CanCollide(const COGCollision& collision, const COGCollision& otherCollision) const;

	bool _CircleBoxCollisionCheck(const COGCircleShape& circle, const COGTransform& transform,
	                              const COGBoxShape& box, const COGTransform& otherTransform) const;

	bool _BoxBoxCollisionCheck(const COGBoxShape& box, const COGTransform& transform,
	                           const COGBoxShape& otherBox, const COGTransform& otherTransform) const;

	std::weak_ptr<World> mWorld;
};
