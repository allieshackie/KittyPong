#pragma once
#include "Component.h"
#include "GameObject.h"
#include <vector>
using namespace std;

class Score : public Component{
public:
	static vector<Score*> mScoreComponents;
public:
	Score(GameObject *pGO);
	// ---- Component Functions -------
	virtual ComponentType GetType() const;
	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Update(float fDeltaT);
	// add to score and getter
	void AddPoint();
	int& GetScore();
private:
	int score;
};