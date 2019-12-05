#pragma once
#include <vector>
#include <algorithm>
#include "Component.h"
#include "Engine/Public/EngineInterface.h"
#include "Engine/Public/SDL.h"

// our game object, it's just a collection of components - that's it!
class GameObject
{
public:

	GameObject(exEngineInterface *pEngine)
	{
		mEngine = pEngine;
	}

	~GameObject()
	{
		for (Component* pComponent : mComponents)
		{
			pComponent->Destroy();

			delete pComponent;
		}
	}

	void Initialize()
	{
		for (Component* pComponent : mComponents)
		{
			pComponent->Initialize();
		}
	}

	void AddComponent(Component* pComponent)
	{
		mComponents.push_back(pComponent);
	}

	template<class T>
	T* FindComponent(ComponentType eType)
	{
		for (Component* pComponent : mComponents)
		{
			if (pComponent->GetType() == eType)
			{
				return (T*)pComponent;
			}
		}

		return nullptr;
	}

	exEngineInterface* GetEngine() { return mEngine; }

private:
	std::vector<Component*> mComponents;
	exEngineInterface *mEngine;
};