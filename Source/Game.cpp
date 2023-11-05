#include "Core/EngineContext.h"
#include "Entity/Systems/COGCollisionSystem.h"
#include "Entity/Systems/COGPhysisSystem.h"

#include "Game.h"

void Game::Init(EngineContext* engine)
{
	mEngine = engine;
	mEngine->LoadFont("PixelLettersFull.ttf");
	mEngine->LoadLevel("kittypong.json");

	mWorld = std::make_unique<World>(*mEngine, mEngine->GetEntityRegistry());

	mPhysicsSystem = std::make_unique<COGPhysicsSystem>();
	mCollisionSystem = std::make_unique<COGCollisionSystem>();
}

void Game::RegisterEntityDescriptions() const
{
}

void Game::Update(float dt) const
{
	if (mWorld->HasGameStarted())
	{
		mPhysicsSystem->Update(dt, mEngine->GetEntityRegistry());
		mCollisionSystem->Update(mEngine->GetEntityRegistry());
		mWorld->Update(*mEngine, mEngine->GetEntityRegistry());
	}
}

void Game::Render()
{
	mWorld->Render(*mEngine);
}
