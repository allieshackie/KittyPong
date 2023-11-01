#include <string>
#include "Core/EngineContext.h"
#include "Entity/EntityRegistry.h"
#include "Entity/Components/Score.h"

#include "ScoreManager.h"

#include "Entity/Components/COGTransform.h"

ScoreManager::ScoreManager(EntityId ballEntity) : mBall(ballEntity)
{
}

bool ScoreManager::Update(const EngineContext& engine, EntityRegistry& entityRegistry, const EntityId& player1,
                          const EntityId& player2)
{
	_DrawKitty(engine);
	// color {100, 0, 0, 255}
	engine.DrawText2D("KITTY PONG", {300, 10}, {10, 10}); // display header
	auto score1 = entityRegistry.GetComponent<Score>(player1);
	auto score2 = entityRegistry.GetComponent<Score>(player2);
	const std::string sc1 = std::to_string(score1.GetScore());
	const std::string sc2 = std::to_string(score2.GetScore());
	engine.DrawText2D(sc1.c_str(), {350, 45}, {10, 10}); // display scores  
	engine.DrawText2D(":", {385, 45}, {10, 10});
	engine.DrawText2D(sc2.c_str(), {400, 45}, {10, 10});
	if (score1.GetScore() == 10 || score2.GetScore() == 10)
	{
		// return true if game ends
		score1.SetScore(0);
		score2.SetScore(0);
		return true;
	}

	return false;
}

// TODO: Haven't implemented color for text
int ScoreManager::Menu(const EngineContext& engine, bool choice1, bool choice2, bool enter)
{
	// menu screen
	if (choice1)
	{
		mInput1 = choice1;
		mInput2 = !choice1;
	}
	if (choice2)
	{
		mInput2 = choice2;
		mInput1 = !choice2;
	}
	engine.DrawText2D("KITTY PONG", {300, 10}, {10, 10});

	_DrawKitty(engine);

	if (mInput1)
	{
		// highlight left box
		engine.DrawBox({150, 300, 0.0f}, {350, 350, 1.0f}, {0, 0, 0});
		// color: {255, 255, 255}
		engine.DrawText2D("1 VS 1", {200, 310}, {10, 10});
		engine.DrawBox({450, 300, 0.0f}, {650, 350, 1.0f}, {255, 255, 255});
		// color: {0, 0, 0}
		engine.DrawText2D("1 VS AI", {500, 310}, {10, 10});
		if (enter)
		{
			// choose 1 v 1
			return 1;
		}
	}
	else if (mInput2)
	{
		// highlight right box
		engine.DrawBox({150, 300, 0.0f}, {350, 350, 1.0f}, {255, 255, 255});
		// color {0,0,0}
		engine.DrawText2D("1 VS 1", {200, 310}, {10, 10});
		engine.DrawBox({450, 300, 0.0f}, {650, 350, 1.0f}, {0, 0, 0});
		// color {255,255,255}
		engine.DrawText2D("1 VS AI", {500, 310}, {10, 10});
		if (enter)
		{
			// choose 1 v AI
			return 2;
		}
	}
	else
	{
		// neither box highlighted
		engine.DrawBox({150, 300, 0.0f}, {350, 350, 1.0f}, {255, 255, 255});
		// color {0,0,0}
		engine.DrawText2D("1 VS 1", {200, 310}, {10, 10});
		engine.DrawBox({450, 300, 0.0f}, {650, 350, 1.0f}, {255, 255, 255});
		// color {0,0,0}
		engine.DrawText2D("1 VS AI", {500, 310}, {10, 10});
	}

	return 0;
}

// TODO: Haven't implemented alpha for color
void ScoreManager::_DrawKitty(const EngineContext& engine)
{
	// head
	// color alpha 155
	engine.DrawCircle({400, 300, 0.0f}, 100, {255, 229, 128});

	//nose
	engine.DrawCircle({390, 325, 0.0f}, 5, {255, 128, 236});
	engine.DrawCircle({400, 330, 0.0f}, 9, {255, 128, 236});
	engine.DrawCircle({410, 325, 0.0f}, 5, {255, 128, 236});

	_DrawMouth(engine);
	_DrawEyes(engine);

	//ears 
	engine.DrawLine({320, 240, 0.0f}, {335, 160, 0.0f}, {0, 0, 0});
	engine.DrawLine({335, 160, 0.0f}, {360, 210, 0.0f}, {0, 0, 0});
	engine.DrawLine({420, 210, 0.0f}, {450, 155, 0.0f}, {0, 0, 0});
	engine.DrawLine({450, 155, 0.0f}, {475, 230, 0.0f}, {0, 0, 0});

	//whiskers left
	engine.DrawLine({370, 320, 0.0f}, {320, 310, 0.0f}, {0, 0, 0});
	engine.DrawLine({370, 330, 0.0f}, {320, 340, 0.0f}, {0, 0, 0});
	engine.DrawLine({372, 345, 0.0f}, {335, 365, 0.0f}, {0, 0, 0});

	// whiskers right
	engine.DrawLine({430, 320, 0.0f}, {480, 310, 0.0f}, {0, 0, 0});
	engine.DrawLine({430, 330, 0.0f}, {480, 332, 0.0f}, {0, 0, 0});
	engine.DrawLine({432, 345, 0.0f}, {470, 360, 0.0f}, {0, 0, 0});
}

void ScoreManager::SetTimer(int amount)
{
	// set timer if player scores
	mTimer = amount;
}

void ScoreManager::_DrawMouth(const EngineContext& engine)
{
	//mouth
	if (mTimer > 0)
	{
		// display surprised mouth for timer duration after player scores
		engine.DrawCircle({400, 350, 0.0f}, 6, {0, 0, 0,});
		mTimer--;
	}
	else
	{
		engine.DrawLine({400, 340, 0.0f}, {385, 350, 0.0f}, {0, 0, 0});
		engine.DrawLine({400, 340, 0.0f}, {415, 350, 0.0f}, {0, 0, 0});
	}
}

void ScoreManager::_DrawEyes(const EngineContext& engine)
{
	// move eye pupils to follow mouse ball
	auto transform = engine.GetEntityRegistry().GetComponent<COGTransform>(mBall);
	const auto& pos = transform.GetPosition();
	if (pos.x < 300)
	{
		// ball left of cat
		mEyePos1.x = 360;
		mEyePos2.x = 420;
	}
	if (pos.x > 500)
	{
		// ball right
		mEyePos1.x = 380;
		mEyePos2.x = 435;
	}
	if (pos.y < 250)
	{
		// ball up
		mEyePos1.y = 275;
		mEyePos2.y = 275;
	}
	if (pos.y > 400)
	{
		// ball down
		mEyePos1.y = 290;
		mEyePos2.y = 290;
	}
	if (pos.x == 200 && pos.y == 200)
	{
		mEyePos1 = {370, 282};
		mEyePos2 = {428, 282};
	}
	//eyes
	engine.DrawCircle({370, 280, 0.0f}, 15, {255, 255, 255});
	engine.DrawCircle(glm::vec3(mEyePos1, 0.0f), 6, {0, 0, 0});
	engine.DrawCircle({428, 280, 0.0f}, 15, {255, 255, 255});
	engine.DrawCircle(glm::vec3(mEyePos2, 0.0f), 6, {0, 0, 0});
}
