#pragma once
#include "Component.h"
#include "GameObject.h"
#include <vector>
using namespace std;

class AIPlayer : public Component {
public:
	static vector<AIPlayer*> mAIComponents;
public:
	AIPlayer(GameObject *pGO);
	// ----- Component functions --------
	virtual ComponentType GetType() const;

	virtual void Initialize() override;

	virtual void Destroy() override;

	virtual void Update(float fDeltaT);

	void SetBall(GameObject *pBall);

private:
	// pointer to ball gameobject
	GameObject *mBall;
};