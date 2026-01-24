#pragma once
#include "COGShape.h"

class COGCircleShape : public COGShape
{
public:
	COGCircleShape(float fRadius, glm::vec4 pColor);

	float GetRadius() const;

	void Render(const RenderSystem& system, COGTransform& transform) override;

private:
	float mRadius = 0.0f;
};
