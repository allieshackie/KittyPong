#pragma once
#include "COGShape.h"

class GameObject;
// COGBoxShape - box
class COGBoxShape : public COGShape
{
public:

	COGBoxShape(GameObject* pGO, float fWidth, float fHeight, exColor pColor);

	virtual ComponentType GetType() const override;

	virtual void Render() override;

	float GetWidth() const;

	float GetHeight() const;

private:

	float mWidth;
	float mHeight;

};