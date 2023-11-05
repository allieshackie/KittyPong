#pragma once
#include "Entity/EntityRegistry.h"

class COGShape;
class COGPhysics;
class COGTransform;

class COGCollisionSystem
{
public:
	void Update(EntityRegistry& entityRegistry) const;

private:
	bool _CircleBoxCollisionCheck(const COGShape& shape, const COGTransform& transform, COGPhysics& physics,
	                              const COGShape& otherShape, const COGTransform& otherTransform) const;
};
