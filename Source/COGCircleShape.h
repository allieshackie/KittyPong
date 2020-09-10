#pragma once
#include "COGShape.h"
class GameObject;

// COGCircleShape - box
class COGCircleShape : public COGShape
{
public:

	COGCircleShape(GameObject* pGO, float fRadius, exColor pColor);

	virtual ComponentType GetType() const override;

	virtual void Render() override;

	float GetRadius() const;

private:

	float mRadius;
};