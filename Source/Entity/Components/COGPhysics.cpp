#include "COGPhysics.h"
#include "COGTransform.h"
#include "Score.h"
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include "COGBoxShape.h"
#include "COGCircleShape.h"
#include "GameObject.h"

std::vector<COGPhysics*> COGPhysics::mPhysicsComponents;

COGPhysics::COGPhysics(GameObject* pGO, bool bGenerateCollisionEvents, exVector2 pVelocity)
	: Component(pGO), mTransform(nullptr), mBoxShape(nullptr), mCircleShape(nullptr),
	mVelocity(pVelocity), normal({ 0,0 }), mGenerateCollisionEvents(bGenerateCollisionEvents)
{
}

ComponentType COGPhysics::GetType() const { return ComponentType::Physics; }

void COGPhysics::Initialize()
{
	mTransform = mGO->FindComponent<COGTransform>(ComponentType::Transform);
	mBoxShape = mGO->FindComponent<COGBoxShape>(ComponentType::BoxShape);
	mCircleShape = mGO->FindComponent<COGCircleShape>(ComponentType::CircleShape);

	AddToComponentVector(mPhysicsComponents);
}

void COGPhysics::Destroy()
{
	RemoveFromComponentVector(mPhysicsComponents);
}

void COGPhysics::Update(float fDeltaT)
{
	// integrate velocity
	exVector2& myPosition = mTransform->GetPosition();

	myPosition.x += mVelocity.x * fDeltaT;
	myPosition.y += mVelocity.y * fDeltaT;

	// check collisions
	if (mGenerateCollisionEvents)
	{
		for (COGPhysics* pPhysicsOther : COGPhysics::mPhysicsComponents)
		{
			// do not collide with self
			if (pPhysicsOther == this)
			{
				continue;
			}

			// TODO - maybe have IsColliding produce a vector/struct that contains the normal and then pass that into OnCollision?
			const bool bResult = IsColliding(pPhysicsOther);

			if (bResult)
			{
				for (IPhysicsCollisionEvent* pCollisionEventListener : mCollisionEventListeners)
				{
					pCollisionEventListener->OnCollision(this, pPhysicsOther);
				}
			}
		}
	}
}

bool COGPhysics::IsColliding(COGPhysics* pOther)
{
	if (mCircleShape != nullptr && pOther->mBoxShape != nullptr)
	{
		// check a circle colliding with a box (paddle)
		auto ball = mGO->FindComponent<COGTransform>(ComponentType::Transform);
		const auto ballShape = mGO->FindComponent<COGCircleShape>(ComponentType::CircleShape);
		auto otherTransform = pOther->mGO->FindComponent<COGTransform>(ComponentType::Transform);
		const auto otherShape = pOther->mGO->FindComponent<COGBoxShape>(ComponentType::BoxShape);
		const exVector2 closest = {max(otherTransform->GetPosition().x, min(ball->GetPosition().x, otherTransform->GetPosition().x + otherShape->GetWidth())),
							max(otherTransform->GetPosition().y,min(ball->GetPosition().y, otherTransform->GetPosition().y + otherShape->GetHeight())) };

		const exVector2 deltaPos = { ball->GetPosition().x - closest.x, ball->GetPosition().y - closest.y };		// get distance from closest point to ball
		const float distance = (pow(deltaPos.x, 2) + pow(deltaPos.y, 2));		// get distance

		if (distance - 2 < ballShape->GetRadius() * 2) {	// if distance is lower than radius
			normal = { deltaPos.x / distance, deltaPos.y / distance };
			auto score = pOther->mGO->FindComponent<Score>(ComponentType::Score);
			// other collider has score component, add to score
			if (score != nullptr) {
				score->AddPoint();
			}
			return true;
		}
	}

	// we don't cover the other cases
	// maybe assert here?

	return false;
}

void COGPhysics::AddCollisionEventListener(IPhysicsCollisionEvent* pEvent)
{
	mCollisionEventListeners.push_back(pEvent);
}