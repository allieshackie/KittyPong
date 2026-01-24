#include "COGTransform.h"

#include "COGBoxShape.h"

COGBoxShape::COGBoxShape(float fWidth, float fHeight, glm::vec4 color)
	: COGShape(color)
	  , mWidth(fWidth)
	  , mHeight(fHeight)
{
}

void COGBoxShape::Render(const RenderSystem& system, COGTransform& transform)
{
	const auto pos = transform.GetPosition();
	system.DrawBox(glm::vec3(pos, 1.0f), {mWidth, mHeight, 1.0f}, GetColor(), true);
}

float COGBoxShape::GetWidth() const
{
	return mWidth;
}

float COGBoxShape::GetHeight() const
{
	return mHeight;
}
