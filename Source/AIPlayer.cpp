#include "AIPlayer.h"
#include "COGTransform.h"
#include <vector>

std::vector<AIPlayer*> AIPlayer::mAIComponents;

AIPlayer::AIPlayer(GameObject *pGO) : Component(pGO), mBall(nullptr) {
}

ComponentType AIPlayer::GetType() const { return ComponentType::AI; }

void AIPlayer::Initialize() {
	AddToComponentVector(mAIComponents);
}

void AIPlayer::Destroy() {
	RemoveFromComponentVector(mAIComponents);
}

// get pointer to the ball so Ai can monitor position
void AIPlayer::SetBall(GameObject *pBall) {
	mBall = pBall;
}

void AIPlayer::Update(float fDeltaT) {
	// get position of the ball and position of the Ai paddle
	auto transform = mBall->FindComponent<COGTransform>(ComponentType::Transform);
	const exVector2& pos = transform->GetPosition();
	exVector2& paddlePos = mGO->FindComponent<COGTransform>(ComponentType::Transform)->GetPosition();
	if (pos.x > 0 && pos.x > 350) { // ball is moving right
		if (paddlePos.y < pos.y - 30) {		// move up
			paddlePos.y += 15;							// intentionally messing up
			paddlePos.y -= 5;
		}
		else if (paddlePos.y > pos.y + 30) {	// move down
			paddlePos.y -= 15;
			paddlePos.y += 7;
		}
	}
}