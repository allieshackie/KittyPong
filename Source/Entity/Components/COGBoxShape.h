#pragma once
#include "COGShape.h"

class GameObject;

class COGBoxShape : public COGShape
{
public:
	COGBoxShape(float fWidth, float fHeight, glm::vec3 color);

	void Render(const EngineContext& context, COGTransform& transform) override;

	float GetWidth() const;
	float GetHeight() const;

private:
	float mWidth;
	float mHeight;
};
