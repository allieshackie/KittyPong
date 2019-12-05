#pragma once
#include "Engine/Public/EngineInterface.h"
#include "Engine/Public/SDL.h"
#include "GameObject.h"
#include "Score.h"
#include "COGTransform.h"
class exEngineInterface;

class ScoreManager {
public:
	ScoreManager(exEngineInterface* pEngine, int ID, Score *player1, Score *player2);
	// --- UPDATE SCORE AND KITTY KITTY ------
	bool Update();
	void AddPoint(int sc1, int sc2);
	int Menu(bool choice1, bool choice2, bool enter);
	void SetTimer(int amount);
	void SetBall(GameObject *pBall);

private:
	// --- DRAW FUNCTIONS FOR KITTY KITTY-----
	void DrawKitty();
	void DrawEyes();
	void DrawMouth();

	Score *score1, *score2;
	int	timer;
	bool input1, input2;
	GameObject *mBall;
	exVector2 eyePos1, eyePos2;
	exEngineInterface* mEngine;
	int fontID;
};