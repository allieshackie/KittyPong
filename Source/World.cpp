#include "World.h"
#include "AIPlayer.h"
#include "COGBounce.h"
#include "COGPhysics.h"
#include "COGBoxShape.h"
#include "COGCircleShape.h"

// create paddle
GameObject* CreatePaddle(exEngineInterface *pEngine, exVector2 position, exColor color, Input *input)
{
	const float fPaddleWidth = 15.0f;
	const float fPaddleHeight = 50.0f;

	auto pPaddle = new GameObject(pEngine);

	auto pTransform = new COGTransform(pPaddle);
	// set the position of the paddle
	exVector2& myPosition = pTransform->GetPosition();
	myPosition = position;
	pPaddle->AddComponent(pTransform);

	const auto pBoxShape = new COGBoxShape(pPaddle, fPaddleWidth, fPaddleHeight, color);
	pPaddle->AddComponent(pBoxShape);

	const auto pPhysics = new COGPhysics(pPaddle, false, {0,0});
	pPaddle->AddComponent(pPhysics);

	// add user input to paddles default
	const auto userIn = new UserInput(pPaddle, input);
	pPaddle->AddComponent(userIn);

	return pPaddle;
}

// create ball (notice how it will generate events)
GameObject* CreateBall(exEngineInterface *pEngine, exVector2 position, exColor color)
{
	const float fBallRadius = 12.0f;

	auto pBall = new GameObject(pEngine);

	auto pTransform = new COGTransform(pBall);
	// set the position of the paddle
	exVector2& myPosition = pTransform->GetPosition();
	myPosition = position;
	pBall->AddComponent(pTransform);

	const auto pCircleShape = new COGCircleShape(pBall, fBallRadius, color);
	pBall->AddComponent(pCircleShape);

	const auto pPhysics = new COGPhysics(pBall, true, {250,150});
	pBall->AddComponent(pPhysics);

	// attach bounce component
	const auto pBounce = new COGBounce(pBall);
	pBall->AddComponent(pBounce);

	return pBall;
}

// create boundaries for the border of the window
GameObject* CreateBoundary(exEngineInterface *pEngine, exVector2 size, exVector2 position, exColor color) {
	auto pBound = new GameObject(pEngine);

	auto pTransform = new COGTransform(pBound);
	exVector2& myPosition = pTransform->GetPosition();
	myPosition = position;
	pBound->AddComponent(pTransform);

	const auto pBoxShape = new COGBoxShape(pBound, size.x, size.y, color);
	pBound->AddComponent(pBoxShape);

	const auto pPhysics = new COGPhysics(pBound, false, { 0,0 });
	pBound->AddComponent(pPhysics);

	return pBound;
}

// sets player 2 after menu choice
void World::SetPlayer2(int choice) {
	// if player vs AI is chosen
	if (choice == 2) {
		// remove user input component
		mGameObjects[6]->FindComponent<UserInput>(ComponentType::Input)->Destroy();
		const auto ai = new AIPlayer(mGameObjects[6]);
		mGameObjects[6]->AddComponent(ai);
		// add AI component and give it a reference to the ball
		mGameObjects[6]->FindComponent<AIPlayer>(ComponentType::AI)->SetBall(mGameObjects[4]);
	}

	// initialize all game objects
	InitializeAll();

}

// initialize all game objects after all components are added/removed
void World::InitializeAll() {
	for (auto object : mGameObjects) {
		object->Initialize();
	}
}

// create score instance
void World::CreateScore(exEngineInterface *pEngine, int fontID) {
	const auto player1 = new Score(mGameObjects[0]);
	// link score component to left boundary
	mGameObjects[0]->AddComponent(player1);
	const auto player2 = new Score(mGameObjects[2]);
	// link score component to right boundary
	mGameObjects[2]->AddComponent(player2);
	// create score manager and link to score components
	score = new ScoreManager(pEngine, fontID, player1, player2);
	score->SetBall(mGameObjects[4]);
}

void World::Create(exEngineInterface *pEngine, int fontID)
{
	player1 = new Input();
	player2 = new Input();
	// boundaries
	mGameObjects.push_back(CreateBoundary(pEngine, { 1,600 }, { 0,0 }, { 0,0,0,255 })); // left
	mGameObjects.push_back(CreateBoundary(pEngine, { 800,1 }, { 0,599 }, { 0,0,0,255 }));
	mGameObjects.push_back(CreateBoundary(pEngine, { 1,600 }, { 799,0 }, { 0,0,0,255 })); // right
	mGameObjects.push_back(CreateBoundary(pEngine, { 800,1 }, { 0,0 }, { 0,0,0,255 }));

	// ball and paddles
	mGameObjects.push_back(CreateBall(pEngine, { 200,200 }, {179,170, 154, 255}));
	mGameObjects.push_back(CreatePaddle(pEngine, { 0,100 }, {255,229,128,255}, player1));
	mGameObjects.push_back(CreatePaddle(pEngine, {780,100}, { 255,229,128,255 }, player2));

	CreateScore(pEngine, fontID);
	
}

void World::Destroy()
{
	for (GameObject* pGameObject : mGameObjects)
	{
		delete pGameObject;
	}

	mGameObjects.clear();
}

void World::Update(float fDeltaT) const
{
	// continually update score
	score->Update();
	// run simulation first
	for (COGPhysics* pPhysics : COGPhysics::mPhysicsComponents)
	{
		pPhysics->Update(fDeltaT);
	}

	// then render everything
	for (COGShape* pShape : COGShape::mShapeComponents)
	{
		pShape->Render();
	}

	// update all user input components
	for (UserInput* input : UserInput::mInputComponents) {
		input->Update(fDeltaT);
	}

	// update all ai components (only if player2 is Ai)
	for (AIPlayer *ai : AIPlayer::mAIComponents) {
		ai->Update(fDeltaT);
	}
}