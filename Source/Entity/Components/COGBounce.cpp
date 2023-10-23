#include "COGBounce.h"
#include "GameObject.h"

ComponentType COGBounce::GetType() const { return ComponentType::Bounce; }

void COGBounce::Initialize()
{
	auto pPhysics = mGO->FindComponent<COGPhysics>(ComponentType::Physics);

	// add myself to the list of objects that are interested in collisions
	// COGPhysics is able to interface with us because we inherit from IPhysicsCollisionEvent
	pPhysics->AddCollisionEventListener(this);
}

void COGBounce::OnCollision(COGPhysics* pMe, COGPhysics* pOther)
{
	const exVector2 normal = pMe->GetNormal();
	exVector2& mVelocity = pMe->GetVelocity();
	// reflect on x if normal is opposite of velocity
	if (normal.x > 0 && mVelocity.x < 0 || normal.x < 0 && mVelocity.x > 0) {
		mVelocity = { -mVelocity.x, mVelocity.y };
	}
	// reflect on y if normal is opposite of velocity
	if (normal.y > 0 && mVelocity.y < 0 || normal.y < 0 && mVelocity.y > 0) {
		mVelocity = { mVelocity.x, -mVelocity.y };
	}
}