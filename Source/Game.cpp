//
// * ENGINE-X
// * SAMPLE GAME
//
// + Game.cpp
// implementation of MyGame, an implementation of exGameInterface
//
#include "Game.h"
#include "World.h"

#include "EngineInterface.h"
#include "SDL_keyboard.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------

const char* gWindowName = "Sample EngineX Game";

//-----------------------------------------------------------------
//-----------------------------------------------------------------

MyGame::MyGame()
	: mEngine( nullptr )
	, mFontID( -1 )
	, mUp( false )
	, mDown( false )
	, wUp(false)
	, sDown(false)
	, choice1(false)
	, choice2(false)
	, enter(false)
	, gameStart(false)
	, mTextPosition({0,0})
{
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

MyGame::~MyGame()
{
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::Initialize( exEngineInterface* pEngine )
{
	mEngine = pEngine;

	mFontID = mEngine->LoadFont( "kitType.ttf", 50 );

	mTextPosition.x = 50.0f;
	mTextPosition.y = 50.0f;
	world.Create(pEngine, mFontID);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

const char* MyGame::GetWindowName() const
{
	return gWindowName;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::GetClearColor( exColor& color ) const
{
	color.mColor[0] = 76;
	color.mColor[1] = 255;
	color.mColor[2] = 224;
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::OnEvent( SDL_Event* pEvent )
{
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::OnEventsConsumed()
{
	int nKeys = 0;
	const Uint8*pState = SDL_GetKeyboardState( &nKeys );

	mUp = pState[SDL_SCANCODE_UP];
	mDown = pState[SDL_SCANCODE_DOWN];
	wUp = pState[SDL_SCANCODE_W];
	sDown = pState[SDL_SCANCODE_S];
	choice1 = pState[SDL_SCANCODE_1];
	choice2 = pState[SDL_SCANCODE_2];
	enter = pState[SDL_SCANCODE_RETURN];
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------

void MyGame::Run( float fDeltaT )
{
	// if game hasn't started, display main menu
	if (!gameStart) {
		const int playerState = world.score->Menu(choice1, choice2, enter);
		if (playerState != 0) {
			gameStart = true;
			// set paddle2 to AI or userInput 
			world.SetPlayer2(playerState);
		}
	}
	else {	// game start, update all user input and world
		world.player1->Update(wUp, sDown);
		world.player2->Update(mUp, mDown);
		world.Update(fDeltaT);
	}
}
