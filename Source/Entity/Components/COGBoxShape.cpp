#include "COGTransform.h"
#include "Core/EngineContext.h"

#include "COGBoxShape.h"

COGBoxShape::COGBoxShape(float fWidth, float fHeight, glm::vec4 color)
	: COGShape(color)
	  , mWidth(fWidth)
	  , mHeight(fHeight)
{
}

void COGBoxShape::Render(const EngineContext& context, COGTransform& transform)
{
	const auto pos = transform.GetPosition();
	context.DrawBox(glm::vec3(pos, 1.0f), {mWidth, mHeight, 1.0f}, GetColor());
}

float COGBoxShape::GetWidth() const
{
	return mWidth;
}

float COGBoxShape::GetHeight() const
{
	return mHeight;
}
