#pragma once
#include "Component.h"
#include "GameObject.h"
#include <vector>

class AIPlayer : public Component {
public:
	static std::vector<AIPlayer*> mAIComponents;
public:
	AIPlayer(GameObject *pGO);
	
	// ----- Component functions --------
	virtual ComponentType GetType() const override;

	virtual void Initialize() override;

	virtual void Destroy() override;

	virtual void Update(float fDeltaT);

	void SetBall(GameObject *pBall);

private:
	// pointer to ball gameobject
	GameObject *mBall;
};