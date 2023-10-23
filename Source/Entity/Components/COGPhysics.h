#pragma once
#include <vector>
#include "IPhysicsCollisionEvent.h"
#include "Component.h"
#include "EngineTypes.h"

class COGTransform;
class COGBoxShape;
class COGCircleShape;
// COGPhysics - lets move around
class COGPhysics : public Component
{
public:

	static std::vector<COGPhysics*> mPhysicsComponents;

public:

	COGPhysics(GameObject* pGO, bool bGenerateCollisionEvents, exVector2 pVelocity);

	virtual ComponentType GetType() const override;

	virtual void Initialize() override;

	virtual void Destroy() override;

	virtual void Update(float fDeltaT);

	bool IsColliding(COGPhysics* pOther);

	void AddCollisionEventListener(IPhysicsCollisionEvent* pEvent);

	exVector2& GetVelocity() { return mVelocity; }

	exVector2 GetNormal() const { return normal; }

private:

	COGTransform* mTransform;
	COGBoxShape* mBoxShape;
	COGCircleShape* mCircleShape;

	exVector2 mVelocity;

	exVector2 normal;

	bool mGenerateCollisionEvents;
	std::vector<IPhysicsCollisionEvent*> mCollisionEventListeners;

};