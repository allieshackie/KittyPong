#include "Score.h"

std::vector<Score*> Score::mScoreComponents;

Score::Score(GameObject *pGO) : Component(pGO), score(0) {
}

ComponentType Score::GetType() const { return ComponentType::Score; };

void Score::Initialize() {
	AddToComponentVector(mScoreComponents);
}

void Score::Destroy() {
	RemoveFromComponentVector(mScoreComponents);
}

// no real update happening
void Score::Update(float fDeltaT) {
	return;
}

// add point to score
void Score::AddPoint() {
	score++;
}

// return score
int& Score::GetScore() {
	return score;
}