//
// * ENGINE-X
// * SAMPLE GAME
//
// + Game.h
// definition of MyGame, an implementation of exGameInterface
//
#pragma once

#include "GameInterface.h"
#include "EngineTypes.h"
#include "World.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------

class MyGame : public exGameInterface
{
public:

								MyGame();
	virtual						~MyGame();

	virtual void				Initialize( exEngineInterface* pEngine ) override;

	virtual const char*			GetWindowName() const override;
	virtual void				GetClearColor( exColor& color ) const override;

	virtual void				OnEvent( SDL_Event* pEvent ) override;
	virtual void				OnEventsConsumed() override;

	virtual void				Run( float fDeltaT ) override;

private:

	exEngineInterface*			mEngine;

	int							mFontID;
	//------- Player Controls --------
	bool						mUp;
	bool						mDown;
	bool						wUp;
	bool						sDown;

	// -------Menu Controls ------
	bool						choice1;
	bool						choice2;
	bool						enter;

	bool						gameStart;

	exVector2					mTextPosition;
	World						world;
};
