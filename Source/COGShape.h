#pragma once
#include <vector>
#include <EngineTypes.h>

#include "Component.h"

// COGShape - our baseclass for shapes
class COGShape : public Component
{
public:

	static std::vector<COGShape*> mShapeComponents;

public:

	COGShape(GameObject* pGO, exColor pColor)
		: Component(pGO), mColor(pColor) {}

	virtual void Initialize() override;

	virtual void Destroy() override;

	virtual void Render() = 0;

	exColor GetColor() const { return mColor; }

private:
	exColor mColor;
};
