#include "Core/EngineContext.h"
#include "Game.h"

void Game::Init(EngineContext* engine)
{
	mEngine = engine;
}

void Game::RegisterEntityDescriptions() const
{
}

void Game::Update(float dt) const
{
	// if game hasn't started, display main menu
	/*
	 *
	if (!gameStart)
	{
		const int playerState = world.score->Menu(choice1, choice2, enter);
		if (playerState != 0)
		{
			gameStart = true;
			// set paddle2 to AI or userInput 
			world.SetPlayer2(playerState);
		}
	}
	else
	{
		// game start, update all user input and world
		world.player1->Update(wUp, sDown);
		world.player2->Update(mUp, mDown);
		world.Update(fDeltaT);
		//mPhysicsSystem->Update(mTimer->mDT);
	}
	 */
}
