#pragma once
#include <glm/vec3.hpp>

class COGTransform;
class EngineContext;

class COGShape
{
public:
	COGShape(glm::vec3 pColor)
		: mColor(pColor)
	{
	}

	virtual ~COGShape() = default;

	glm::vec3 GetColor() const { return mColor; }

	virtual void Render(const EngineContext& context, COGTransform& transform)
	{
	}

private:
	glm::vec3 mColor = {0, 0, 0};
};
