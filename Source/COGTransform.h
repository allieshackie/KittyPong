#pragma once

#include <EngineTypes.h>

#include "Component.h"

// COGTransform - where we are in space
class COGTransform : public Component
{
public:

	COGTransform(GameObject* pGO)
		: Component(pGO), mPosition({ 0, 0 })
	{
	}

	virtual ComponentType GetType() const { return ComponentType::Transform; }

	exVector2& GetPosition() { return mPosition; }

	void SetPosition(exVector2 newPos) { mPosition = newPos; }

private:

	exVector2 mPosition;

};