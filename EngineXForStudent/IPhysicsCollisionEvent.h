#pragma once

class COGPhysics;


class IPhysicsCollisionEvent
{
public:

	virtual void OnCollision(COGPhysics* pMe, COGPhysics* pOther) = 0;

};