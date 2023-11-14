#pragma once
#include "Entity/EntityRegistry.h"

class COGBoxShape;
class COGCircleShape;
class COGPhysics;
class COGTransform;

class COGCollisionSystem
{
public:
	void Update(EntityRegistry& entityRegistry) const;

private:
	bool _CircleBoxCollisionCheck(const COGCircleShape& circle, const COGTransform& transform,
	                              const COGBoxShape& box, const COGTransform& otherTransform) const;
};
