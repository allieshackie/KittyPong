#pragma once
#include <glm/vec4.hpp>

class COGTransform;
class EngineContext;

class COGShape
{
public:
	COGShape(glm::vec4 pColor)
		: mColor(pColor)
	{
	}

	virtual ~COGShape() = default;

	glm::vec4 GetColor() const { return mColor; }

	virtual void Render(const EngineContext& context, COGTransform& transform)
	{
	}

private:
	glm::vec4 mColor = {0, 0, 0, 1};
};
