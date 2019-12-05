#pragma once
#include "GameObject.h"
#include "COGTransform.h"
#include "COGBoxShape.h"
#include "COGPhysics.h"
#include "COGBounce.h"
#include "Score.h"
#include "UserInput.h"
#include "ScoreManager.h"
#include "AIPlayer.h"
#include "Engine/Public/EngineInterface.h"
#include "Engine/Public/SDL.h"
#include <vector>
using namespace std;

// the world, it contains all of our game objects and defines the order of updates
class World
{
public:
	void Create(exEngineInterface *pEngine, int fontID);

	void Destroy();

	void Update(float fDeltaT);

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