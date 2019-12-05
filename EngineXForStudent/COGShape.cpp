
#include "COGShape.h"

std::vector<COGShape*> COGShape::mShapeComponents;

void COGShape::Initialize()
{
	AddToComponentVector(mShapeComponents);
}

void COGShape::Destroy()
{
	RemoveFromComponentVector(mShapeComponents);
}