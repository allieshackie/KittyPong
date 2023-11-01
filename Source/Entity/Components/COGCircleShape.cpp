#include "COGTransform.h"
#include "Core/EngineContext.h"
#include "COGCircleShape.h"

COGCircleShape::COGCircleShape(float fRadius, glm::vec3 pColor)
	: COGShape(pColor)
	  , mRadius(fRadius)
{
}

void COGCircleShape::Render(const EngineContext& context, COGTransform& transform)
{
	const auto& pos = transform.GetPosition();
	context.DrawCircle(glm::vec3(pos, 1.0f), mRadius, GetColor());
}

float COGCircleShape::GetRadius() const
{
	return mRadius;
}
