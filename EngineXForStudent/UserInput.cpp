#include "UserInput.h"
#include <vector>

std::vector<UserInput*> UserInput::mInputComponents;

UserInput::UserInput(GameObject* pGO, Input *input) : Component(pGO) {
	mInput = input;
}

ComponentType UserInput::GetType() const { return ComponentType::Input; }

void UserInput::Initialize() {
	AddToComponentVector(mInputComponents);
}

void UserInput::Destroy() {
	RemoveFromComponentVector(mInputComponents);
}

void UserInput::Update(float fDeltaT) {
	// get position of game object
	COGTransform *transform = mGO->FindComponent<COGTransform>(ComponentType::Transform);
	exVector2 position = transform->GetPosition();

	// if input is up
	if (mInput->up) {
		position.y -= 300 * fDeltaT;
		transform->SetPosition(position);
	}
	// if input is down
	if (mInput->down) {
		position.y += 300 * fDeltaT;
		transform->SetPosition(position);
	}
}