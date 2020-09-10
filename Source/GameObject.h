#pragma once
#include <vector>
#include "Component.h"
#include "EngineInterface.h"

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
				return static_cast<T*>(pComponent);
			}
		}

		return nullptr;
	}

	exEngineInterface* GetEngine() const { return mEngine; }

private:
	std::vector<Component*> mComponents;
	exEngineInterface *mEngine;
};