#pragma once
#include "COGShape.h"


// COGBoxShape - box
class COGBoxShape : public COGShape
{
public:

	COGBoxShape(GameObject* pGO, float fWidth, float fHeight, exColor pColor);

	virtual ComponentType GetType() const;

	virtual void Render() override;

	float GetWidth();

	float GetHeight();

private:

	float mWidth;
	float mHeight;

};