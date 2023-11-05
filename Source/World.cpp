#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Core/EngineContext.h"
#include "Entity/EntityRegistry.h"
#include "Entity/Player.h"
#include "Entity/Components/AIPlayer.h"
#include "Entity/Components/COGBounce.h"
#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCircleShape.h"
#include "Entity/Components/COGCollision.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Systems/ScoreManager.h"
#include "LLGL/Key.h"

#include "World.h"

World::World(const EngineContext& engine, EntityRegistry& registry)
{
	mPlayer1 = std::make_unique<Player>(registry, glm::vec2{0, 100}, glm::vec3{255, 229, 128}, glm::vec2{1, 600},
	                                    glm::vec2{799, 0}, glm::vec3{0, 0, 255});

	mPlayer2 = std::make_unique<Player>(registry, glm::vec2{780, 100}, glm::vec3{255, 229, 128}, glm::vec2{1, 600},
	                                    glm::vec2{0, 0}, glm::vec3{0, 0, 255});
	// boundaries
	const auto boundary1 = registry.CreateEntity();
	registry.AddComponent<COGTransform>(boundary1, glm::vec2{800, 1});
	registry.AddComponent<COGBoxShape>(boundary1, 0.0f, 599.0f, glm::vec3{0, 0, 255});
	registry.AddComponent<COGPhysics>(boundary1, glm::vec2{0, 0});
	registry.AddComponent<COGCollision>(boundary1);

	const auto boundary2 = registry.CreateEntity();
	registry.AddComponent<COGTransform>(boundary2, glm::vec2{800, 1});
	registry.AddComponent<COGBoxShape>(boundary2, 0.0f, 0.0f, glm::vec3{0, 0, 255});
	registry.AddComponent<COGPhysics>(boundary2, glm::vec2{0, 0});
	registry.AddComponent<COGCollision>(boundary2);

	mBall = _CreateBall(registry, {200, 200}, {179, 170, 154});
	mScoreManager = std::make_unique<ScoreManager>();

	engine.GetInputHandler().RegisterButtonDownHandler(LLGL::Key::Keypad1, [=]() { _ChooseButton1(); });
	engine.GetInputHandler().RegisterButtonDownHandler(LLGL::Key::Keypad2, [=]() { _ChooseButton2(); });
	engine.GetInputHandler().RegisterButtonDownHandler(LLGL::Key::Return, [=]() { _ChooseButtonEnter(); });
}

void World::Update(const EngineContext& engine, EntityRegistry& registry) const
{
	// continually update score
	mScoreManager->Update(engine, registry, mPlayer1->GetPlayerId(), mPlayer2->GetPlayerId());
}

// TODO: Haven't implemented color for text
void World::Render(const EngineContext& engine)
{
	if (!mHasGameStarted)
	{
		_Menu(engine);
	}
	else
	{
		_DrawKitty(engine);
	}
}

void World::SetPlayerAI(EntityRegistry& registry) const
{
	const auto id = mPlayer2->GetPlayerId();
	registry.AddComponent<AIPlayer>(id);
}

void World::_Menu(const EngineContext& engine) const
{
	engine.DrawText2D("KITTY PONG", {300, 10}, {2, 2});

	if (mInput1)
	{
		// highlight left box
		engine.DrawBox({150, 300, 0.0f}, {350, 350, 1.0f}, {0, 0, 0});
		// color: {255, 255, 255}
		engine.DrawText2D("1 VS 1", {200, 310}, {1, 1});
		engine.DrawBox({450, 300, 0.0f}, {650, 350, 1.0f}, {255, 255, 255});
		// color: {0, 0, 0}
		engine.DrawText2D("1 VS AI", {500, 310}, {1, 1});
	}
	else if (mInput2)
	{
		// highlight right box
		engine.DrawBox({150, 300, 0.0f}, {350, 350, 1.0f}, {255, 255, 255});
		// color {0,0,0}
		engine.DrawText2D("1 VS 1", {200, 310}, {1, 1});
		engine.DrawBox({450, 300, 0.0f}, {650, 350, 1.0f}, {0, 0, 0});
		// color {255,255,255}
		engine.DrawText2D("1 VS AI", {500, 310}, {1, 1});
	}
	else
	{
		// neither box highlighted
		engine.DrawBox({150, 300, 0.0f}, {350, 350, 1.0f}, {255, 255, 255});
		// color {0,0,0}
		engine.DrawText2D("1 VS 1", {200, 310}, {1, 1});
		engine.DrawBox({450, 300, 0.0f}, {650, 350, 1.0f}, {255, 255, 255});
		// color {0,0,0}
		engine.DrawText2D("1 VS AI", {500, 310}, {1, 1});
	}
}

EntityId World::_CreateBall(EntityRegistry& registry, glm::vec2 position, glm::vec3 color) const
{
	const auto ballEntity = registry.CreateEntity();
	registry.AddComponent<COGTransform>(ballEntity, position);
	registry.AddComponent<COGCircleShape>(ballEntity, fBallRadius, color);
	registry.AddComponent<COGPhysics>(ballEntity, glm::vec2{250, 150});
	registry.AddComponent<COGBounce>(ballEntity);

	return ballEntity;
}

void World::_DrawMouth(const EngineContext& engine)
{
	//mouth
	if (mScoreAnimationTimer > 0)
	{
		// display surprised mouth for timer duration after player scores
		engine.DrawCircle({400, 350, 0.0f}, 6, {0, 0, 0,});
		mScoreAnimationTimer--;
	}
	else
	{
		engine.DrawLine({400, 340, 0.0f}, {385, 350, 0.0f}, {0, 0, 0});
		engine.DrawLine({400, 340, 0.0f}, {415, 350, 0.0f}, {0, 0, 0});
	}
}

void World::_ChooseButton1()
{
	mInput1 = true;
	mInput2 = false;
}

void World::_ChooseButton2()
{
	mInput1 = false;
	mInput2 = true;
}

void World::_ChooseButtonEnter()
{
	mHasGameStarted = true;
}

// TODO: Haven't implemented alpha for color
void World::_DrawKitty(const EngineContext& engine)
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


void World::_DrawEyes(const EngineContext& engine)
{
	// move eye pupils to follow mouse ball
	const auto& transform = engine.GetEntityRegistry().GetComponent<COGTransform>(mBall);
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
	if (pos.x == 200.0f && pos.y == 200.0f)
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
