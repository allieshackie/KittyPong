#include "ScoreManager.h"
#include <string>

#include "COGTransform.h"
using namespace std;

ScoreManager::ScoreManager(exEngineInterface* pEngine, int ID, Score *player1, Score *player2) {
	mEngine = pEngine;
	score1 = 0;		// initialize scores
	score2 = 0;
	timer = 0;
	eyePos1 = { 370, 282 };		// initialize eye pupil positions
	eyePos2 = { 428,282 };
	fontID = ID;
	input1 = false;
	input2 = false;
	score1 = player1;
	score2 = player2;
}

int ScoreManager::Menu(bool choice1, bool choice2, bool enter) {	// menu screen
	if (choice1) {
		input1 = choice1;
		input2 = !choice1;
	}
	if (choice2) {
		input2 = choice2;
		input1 = !choice2;
	}
	mEngine->DrawText(fontID, { 300,10 }, "KITTY PONG", { 100,0,0,255 }, 0);
	DrawKitty();
	if (input1) {	// highlight left box
		mEngine->DrawBox({ 150,300 }, { 350, 350 }, { 0,0,0,255 }, 0);
		mEngine->DrawText(fontID, { 200, 310 }, "1 VS 1", { 255,255,255,255 }, 1);
		mEngine->DrawBox({ 450,300 }, { 650, 350 }, { 255,255,255,255 }, 0);
		mEngine->DrawText(fontID, { 500, 310 }, "1 VS AI", { 0,0,0,255 }, 1);
		if (enter) {	// choose 1 v 1
			return 1;
		}
	}
	else if (input2) {	// highlight right box
		mEngine->DrawBox({ 150,300 }, { 350, 350 }, { 255,255,255,255 }, 0);
		mEngine->DrawText(fontID, { 200, 310 }, "1 VS 1", { 0,0,0,255 }, 1);
		mEngine->DrawBox({ 450,300 }, { 650, 350 }, { 0,0,0,255 }, 0);
		mEngine->DrawText(fontID, { 500, 310 }, "1 VS AI", { 255,255,255,255 }, 1);
		if (enter) {		// choose 1 v AI
			return 2;
		}
	}
	else {	// neither box highlighted
		mEngine->DrawBox({ 150,300 }, { 350, 350 }, { 255,255,255,255 }, 0);
		mEngine->DrawText(fontID, { 200, 310 }, "1 VS 1", { 0,0,0,255 }, 1);
		mEngine->DrawBox({ 450,300 }, { 650, 350 }, { 255,255,255,255 }, 0);
		mEngine->DrawText(fontID, { 500, 310 }, "1 VS AI", { 0,0,0,255 }, 1);
	}

	return 0;
}

void ScoreManager::DrawKitty() {
	// head
	mEngine->DrawCircle({ 400,300 }, 100, { 255,229,128,155 }, 0);
	//nose
	mEngine->DrawCircle({ 390,325 }, 5, { 255,128,236,255 }, 0);
	mEngine->DrawCircle({ 400,330 }, 9, { 255,128,236,255 }, 0);
	mEngine->DrawCircle({ 410,325 }, 5, { 255,128,236,255 }, 0);
	DrawMouth();
	DrawEyes();
	//ears 
	mEngine->DrawLine({ 320,240 }, { 335,160 }, { 0,0,0,255 }, 0);
	mEngine->DrawLine({ 335,160 }, { 360,210 }, { 0,0,0,255 }, 0);
	mEngine->DrawLine({ 420,210 }, { 450,155 }, { 0,0,0,255 }, 0);
	mEngine->DrawLine({ 450,155 }, { 475, 230 }, { 0, 0, 0, 255 }, 0);
	//whiskers left
	mEngine->DrawLine({ 370,320 }, { 320,310 }, { 0,0,0,255 }, 0);
	mEngine->DrawLine({ 370,330 }, { 320,340 }, { 0,0,0,255 }, 0);
	mEngine->DrawLine({ 372,345 }, { 335,365 }, { 0,0,0,255 }, 0);
	// whiskers right
	mEngine->DrawLine({ 430,320 }, { 480,310 }, { 0,0,0,255 }, 0);
	mEngine->DrawLine({ 430,330 }, { 480,332 }, { 0,0,0,255 }, 0);
	mEngine->DrawLine({ 432,345 }, { 470,360 }, { 0,0,0,255 }, 0);
}

void ScoreManager::SetTimer(int amount) {	// set timer if player scores
	timer = amount;
}

void ScoreManager::DrawMouth() {
	//mouth
	if (timer > 0) {	// display surprised mouth for timer duration after player scores
		mEngine->DrawCircle({ 400,350 }, 6, { 0,0,0,255 }, 0);
		timer--;
	}
	else {
		mEngine->DrawLine({ 400,340 }, { 385,350 }, { 0,0,0,255 }, 0);
		mEngine->DrawLine({ 400,340 }, { 415,350 }, { 0,0,0,255 }, 0);
	}
}

void ScoreManager::DrawEyes() {		// move eye pupils to follow mouse ball
	auto transform = mBall->FindComponent<COGTransform>(ComponentType::Transform);
	const exVector2 pos = transform->GetPosition();
	if (pos.x < 300) { // ball left of cat
		eyePos1.x = 360;
		eyePos2.x = 420;
	}
	if (pos.x > 500) { // ball right
		eyePos1.x = 380;
		eyePos2.x = 435;
	}
	if (pos.y < 250) { // ball up
		eyePos1.y = 275;
		eyePos2.y = 275;
	}
	if (pos.y > 400) { // ball down
		eyePos1.y = 290;
		eyePos2.y = 290;
	}
	if (pos.x == 200 && pos.y == 200) {
		eyePos1 = { 370, 282 };
		eyePos2 = { 428,282 };
	}
	//eyes
	mEngine->DrawCircle({ 370, 280 }, 15, { 255,255,255,255 }, 0);
	mEngine->DrawCircle(eyePos1, 6, { 0,0,0,255 }, 0);
	mEngine->DrawCircle({ 428,280 }, 15, { 255,255,255,255 }, 0);
	mEngine->DrawCircle(eyePos2, 6, { 0,0,0,255 }, 0);
}

void ScoreManager::AddPoint(int sc1, int sc2) {	// add point if a player scores
	score1 += sc1;
	score2 += sc2;
}

void ScoreManager::SetBall(GameObject *pBall) {
	mBall = pBall;
}

bool ScoreManager::Update() {
	DrawKitty();
	mEngine->DrawText(fontID, { 300,10 }, "KITTY PONG", { 100,0,0,255 }, 0);	// display header
	const string sc1 = to_string(score1->GetScore());
	const string sc2 = to_string(score2->GetScore());
	mEngine->DrawText(fontID, { 350,45 }, sc1.c_str(), { 100,0,0,255 }, 0);		// display scores
	mEngine->DrawText(fontID, { 385,45 }, ":", { 100,0,0,255 }, 0);
	mEngine->DrawText(fontID, { 400,45 }, sc2.c_str(), { 100,0,0,255 }, 0);
	if (score1->GetScore() == 10 || score2->GetScore() == 10) {		// return true if game ends
		score1->GetScore() = 0;
		score2->GetScore() = 0;
		return true;
	}

	return false;
}