#pragma once
#include "Component.h"
#include <vector>

class Score : public Component{
public:
	static std::vector<Score*> mScoreComponents;
public:
	Score(GameObject *pGO);
	// ---- Component Functions -------
	virtual ComponentType GetType() const override;
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update(float fDeltaT);
	// add to score and getter
	void AddPoint();
	int& GetScore();
private:
	int score;
};