#pragma once
#include "COGShape.h"

// COGCircleShape - box
class COGCircleShape : public COGShape
{
public:

	COGCircleShape(GameObject* pGO, float fRadius, exColor pColor);

	virtual ComponentType GetType() const;

	virtual void Render() override;

	float GetRadius();

private:

	float mRadius;
};