#pragma once
#include "GameObject.h"
#include "COGTransform.h"
#include "UserInput.h"
#include "ScoreManager.h"
#include "EngineInterface.h"
#include <vector>

// the world, it contains all of our game objects and defines the order of updates
class World
{
public:
	void Create(exEngineInterface *pEngine, int fontID);

	void Destroy();

	void Update(float fDeltaT) const;

	void InitializeAll();

	void SetPlayer2(int choice);

	void CreateScore(exEngineInterface *pEngine, int fontID);

	// user input and score manager 
	Input *player1;
	Input *player2;
	ScoreManager *score;
private:
	std::vector<GameObject*> mGameObjects;
};