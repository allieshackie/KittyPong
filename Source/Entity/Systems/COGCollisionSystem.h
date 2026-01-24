#pragma once

class COGCollision;
class COGBoxShape;
class COGCircleShape;
class COGPhysics;
class COGTransform;
class World;

class COGCollisionSystem
{
public:
	void Update(std::weak_ptr<World> world) const;

private:
	bool _CanCollide(const COGCollision& collision, const COGCollision& otherCollision) const;

	bool _CircleBoxCollisionCheck(const COGCircleShape& circle, const COGTransform& transform,
	                              const COGBoxShape& box, const COGTransform& otherTransform) const;

	bool _BoxBoxCollisionCheck(const COGBoxShape& box, const COGTransform& transform,
	                           const COGBoxShape& otherBox, const COGTransform& otherTransform) const;
};
