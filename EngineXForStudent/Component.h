#pragma once
#include <vector>
#include <algorithm>

#include "Engine/Public/EngineInterface.h"
#include "Engine/Public/SDL.h"
using namespace std;

class GameObject;

struct Vector2
{
	float x;
	float y;
};

// all the different types of components
enum class ComponentType : int
{
	Transform = 0,
	Physics,
	BoxShape,
	CircleShape,
	Bounce,
	Input,
	AI,
	Score
};

// our base component class
class Component
{
public:

	Component(GameObject* pGO)
		: mGO(pGO)
	{
	}

	virtual ComponentType GetType() const = 0;

	virtual void Initialize() { }
	virtual void Destroy() { }

protected:

	template<class T>
	void AddToComponentVector(std::vector<T*>& componentVector)
	{
		componentVector.push_back((T*)this);
	}

	template<class T>
	void RemoveFromComponentVector(std::vector<T*>& componentVector)
	{
		componentVector.erase(std::remove(componentVector.begin(), componentVector.end(), this), componentVector.end());
	}


protected:

	GameObject* mGO;

};