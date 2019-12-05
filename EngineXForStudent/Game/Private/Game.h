//
// * ENGINE-X
// * SAMPLE GAME
//
// + Game.h
// definition of MyGame, an implementation of exGameInterface
//

#include "Game/Public/GameInterface.h"
#include "Engine/Public/EngineTypes.h"
#include "World.h"

//-----------------------------------------------------------------
//-----------------------------------------------------------------

class MyGame : public exGameInterface
{
public:

								MyGame();
	virtual						~MyGame();

	virtual void				Initialize( exEngineInterface* pEngine );

	virtual const char*			GetWindowName() const;
	virtual void				GetClearColor( exColor& color ) const;

	virtual void				OnEvent( SDL_Event* pEvent );
	virtual void				OnEventsConsumed();

	virtual void				Run( float fDeltaT );

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
