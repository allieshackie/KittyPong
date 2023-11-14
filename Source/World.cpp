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
	mPlayer1 = std::make_unique<Player>(registry, glm::vec2{-390, 100}, glm::vec3{255, 229, 128}, glm::vec2{1, 1200},
	                                    glm::vec2{400, 0}, glm::vec3{0, 0, 255});

	mPlayer2 = std::make_unique<Player>(registry, glm::vec2{390, 100}, glm::vec3{255, 229, 128}, glm::vec2{1, 1200},
	                                    glm::vec2{-399, 0}, glm::vec3{0, 0, 255});
	// boundaries
	/*
	 *
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
	 */

	mBall = _CreateBall(registry, {0, 0}, {179, 170, 154});
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
void World::Render(const EngineContext& engine, EntityRegistry& registry)
{
	if (!mHasGameStarted)
	{
		_Menu(engine);
	}
	else
	{
		const auto circleView = registry.GetEnttRegistry().view<COGCircleShape, COGTransform>();
		circleView.each([&engine](auto& shape, auto& transform)
		{
			shape.Render(engine, transform);
		});
		const auto boxView = registry.GetEnttRegistry().view<COGBoxShape, COGTransform>();
		boxView.each([&engine](auto& shape, auto& transform)
		{
			shape.Render(engine, transform);
		});
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
	engine.DrawText2D("KITTY PONG", {200, 10}, {2, 2});

	engine.DrawText2D("1 VS 1", {200, 310}, {1, 1});
	// color: {0, 0, 0}
	engine.DrawText2D("1 VS AI", {500, 310}, {1, 1});
	if (mInput1)
	{
		// highlight left box
		engine.DrawBox({-110, -310, 0.0f}, {200, 100, 1.0f}, {1.0f, 1.0f, 1.0f});
	}
	else if (mInput2)
	{
		// highlight right box
		engine.DrawBox({400, -310, 0.0f}, {230, 100, 1.0f}, {1.0f, 1.0f, 1.0f});
	}

	engine.DrawText2D("Press 1 or 2 to select", {320, 500}, {0.6, 0.6});
}

EntityId World::_CreateBall(EntityRegistry& registry, glm::vec2 position, glm::vec3 color) const
{
	const auto ballEntity = registry.CreateEntity();
	registry.AddComponent<COGTransform>(ballEntity, position);
	registry.AddComponent<COGCircleShape>(ballEntity, fBallRadius, color);
	registry.AddComponent<COGPhysics>(ballEntity, glm::vec2{50, 50});
	registry.AddComponent<COGBounce>(ballEntity);

	return ballEntity;
}

void World::_DrawMouth(const EngineContext& engine)
{
	//mouth
	if (mScoreAnimationTimer > 0)
	{
		// display surprised mouth for timer duration after player scores
		engine.DrawCircle({4.00f, 3.50f, 0.0f}, 0.2f, {255, 255, 255});
		mScoreAnimationTimer--;
	}
	else
	{
		engine.DrawLine({4.00f, 3.40f, 0.0f}, {3.85f, 3.50f, 0.0f}, {255, 255, 255});
		engine.DrawLine({4.00f, 3.40f, 0.0f}, {4.15f, 3.50f, 0.0f}, {255, 255, 255});
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
	if (mInput1 || mInput2)
	{
		mHasGameStarted = true;
	}
}

// TODO: Haven't implemented alpha for color
void World::_DrawKitty(const EngineContext& engine)
{
	// head
	// color alpha 155
	engine.DrawCircle({0.0f, 300.0f, 0.0f}, 150.0f, {1.0f, 229, 0.5f});

	//nose
	engine.DrawCircle({-15.0f, 310.0f, 0.0f}, 10.0f, {1.0f, 0.5f, 0.8f});
	engine.DrawCircle({0.0f, 300.0f, 0.0f}, 14.0f, {1.0f, 0.5f, 0.8f});
	engine.DrawCircle({15.0f, 310.0f, 0.0f}, 10.0f, {1.0f, 0.5f, 0.8f});

	_DrawMouth(engine);
	_DrawEyes(engine);

	//ears 
	engine.DrawLine({-100.0, 100.0f, 0.0f}, {-95.0f, 50.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
	engine.DrawLine({-95.0f, 50.0f, 0.0f}, {-105.0, 100.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
	engine.DrawLine({4.20f, 2.10f, 0.0f}, {4.50f, 1.55f, 0.0f}, {1.0f, 1.0f, 1.0f});
	engine.DrawLine({4.50f, 1.55f, 0.0f}, {4.75f, 2.30f, 0.0f}, {1.0f, 1.0f, 1.0f});

	//whiskers left
	engine.DrawLine({3.70f, 3.20f, 0.0f}, {3.20f, 3.10f, 0.0f}, {1.0f, 1.0f, 1.0f});
	engine.DrawLine({3.70f, 3.30f, 0.0f}, {3.20f, 3.40f, 0.0f}, {1.0f, 1.0f, 1.0f});
	engine.DrawLine({3.72f, 3.45f, 0.0f}, {3.35f, 3.65f, 0.0f}, {1.0f, 1.0f, 1.0f});

	// whiskers right
	engine.DrawLine({4.30f, 3.20f, 0.0f}, {4.80f, 3.10f, 0.0f}, {1.0f, 1.0f, 1.0f});
	engine.DrawLine({4.30f, 3.30f, 0.0f}, {4.80f, 3.32f, 0.0f}, {1.0f, 1.0f, 1.0f});
	engine.DrawLine({4.32f, 3.45f, 0.0f}, {4.70f, 3.60f, 0.0f}, {1.0f, 1.0f, 1.0f});
}


void World::_DrawEyes(const EngineContext& engine)
{
	// move eye pupils to follow mouse ball
	const auto& transform = engine.GetEntityRegistry().GetComponent<COGTransform>(mBall);
	const auto& pos = transform.GetPosition();
	if (pos.x < 300.0f)
	{
		// ball left of cat
		mEyePos1.x = -45.0f;
		mEyePos2.x = 35.0f;
	}
	if (pos.x > 500.0f)
	{
		// ball right
		mEyePos1.x = -35.0f;
		mEyePos2.x = 45.0f;
	}
	if (pos.y < 250.0f)
	{
		// ball up
		mEyePos1.y = 245.0f;
		mEyePos2.y = 245.0f;
	}
	if (pos.y > 400.0f)
	{
		// ball down
		mEyePos1.y = 255.0f;
		mEyePos2.y = 255.0f;
	}
	if (pos.x == 200.0f && pos.y == 200.0f)
	{
		mEyePos1 = {-40.0f, 250};
		mEyePos2 = {40.0f, 250};
	}
	//eyes
	engine.DrawCircle({50.0f, 250.0f, 0.0f}, 30.0f, {1.0f, 1.0f, 1.0f});
	engine.DrawCircle({-50.0f, 250.0f, 0.0f}, 30.0f, {1.0f, 1.0f, 1.0f});
	engine.DrawCircle(glm::vec3(mEyePos1, 0.0f), 10.0f, {0.0f, 0.0f, 0.0f});
	engine.DrawCircle(glm::vec3(mEyePos2, 0.0f), 10.0f, {0.0f, 0.0f, 0.0f});
}
