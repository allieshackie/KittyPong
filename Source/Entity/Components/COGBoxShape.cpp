#include "COGBoxShape.h"
#include "COGTransform.h"
#include "GameObject.h"

COGBoxShape::COGBoxShape(GameObject* pGO, float fWidth, float fHeight, exColor pColor)
	: COGShape(pGO, pColor)
	, mWidth(fWidth)
	, mHeight(fHeight)
{
}

ComponentType COGBoxShape::GetType() const { return ComponentType::BoxShape; }

void COGBoxShape::Render()
{
	auto pTransform = mGO->FindComponent<COGTransform>(ComponentType::Transform);

	// tell EngineX to draw me
	const exVector2 transform = pTransform->GetPosition();
	mGO->GetEngine()->DrawBox(transform, { transform.x + mWidth, transform.y + mHeight }, GetColor(), 1);
}

float COGBoxShape::GetWidth() const {
	return mWidth;
}

float COGBoxShape::GetHeight() const {
	return mHeight;
}