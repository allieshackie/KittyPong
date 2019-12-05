#pragma once
#include <vector>
#include <algorithm>
#include "Component.h"
#include "GameObject.h"
#include "Engine/Public/EngineInterface.h"
#include "Engine/Public/SDL.h"
using namespace std;

// COGShape - our baseclass for shapes
class COGShape : public Component
{
public:

	static std::vector<COGShape*> mShapeComponents;

public:

	COGShape(GameObject* pGO, exColor pColor)
		: Component(pGO)
	{
		mColor = pColor;
	}

	virtual void Initialize() override;

	virtual void Destroy() override;

	virtual void Render() = 0;

	exColor GetColor() { return mColor; }

private:
	exColor mColor;
};
