#pragma once
#include "Component.h"
#include "GameObject.h"
#include "COGTransform.h"
#include "Input.h"
#include <vector>
using namespace std;

class UserInput : public Component {
public:
	static vector<UserInput*> mInputComponents;
public:
	// get pointer to input object that has updated bools
	UserInput(GameObject* pGO, Input *input);
	// ---- Component functions ------
	virtual ComponentType GetType() const;

	virtual void Initialize() override;

	virtual void Destroy() override;

	// update paddle transform
	virtual void Update(float fDeltaT);

private:
	// pointer to input instance that monitors bools for that gameobject
	Input *mInput;
};