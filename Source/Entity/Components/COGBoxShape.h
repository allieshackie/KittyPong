#pragma once
#include "COGShape.h"

class RenderSystem;

class COGBoxShape : public COGShape
{
public:
	COGBoxShape(float fWidth, float fHeight, glm::vec4 color);

	void Render(const RenderSystem& system, COGTransform& transform) override;

	float GetWidth() const;
	float GetHeight() const;

private:
	float mWidth;
	float mHeight;
};
