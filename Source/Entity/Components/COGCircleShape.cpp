#include "COGTransform.h"
#include "COGCircleShape.h"

COGCircleShape::COGCircleShape(float fRadius, glm::vec4 pColor)
	: COGShape(pColor)
	  , mRadius(fRadius)
{
}

void COGCircleShape::Render(const RenderSystem& system, COGTransform& transform)
{
	const auto& pos = transform.GetPosition();
	system.DrawCircle(glm::vec3(pos, 1.0f), mRadius, GetColor());
}

float COGCircleShape::GetRadius() const
{
	return mRadius;
}
