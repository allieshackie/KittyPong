#include "COGCircleShape.h"
#include "COGTransform.h"
#include "GameObject.h"

COGCircleShape::COGCircleShape(GameObject* pGO, float fRadius, exColor pColor)
	: COGShape(pGO, pColor)
	, mRadius(fRadius)
{
}

ComponentType COGCircleShape::GetType() const { return ComponentType::CircleShape; }

void COGCircleShape::Render()
{
	// tell EngineX to draw me
	auto pTransform = mGO->FindComponent<COGTransform>(ComponentType::Transform);

	// tell EngineX to draw me
	const exVector2 transform = pTransform->GetPosition();
	mGO->GetEngine()->DrawCircle(transform, mRadius, GetColor(), 1);
}

float COGCircleShape::GetRadius() const {
	return mRadius;
}