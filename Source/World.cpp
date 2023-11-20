#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "LLGL/Key.h"

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

#include "World.h"

World::World(const EngineContext& engine, EntityRegistry& registry)
{
	engine.GetInputHandler().RegisterButtonDownHandler(LLGL::Key::Keypad1, [=]() { _ChooseButton1(); });
	engine.GetInputHandler().RegisterButtonDownHandler(LLGL::Key::Keypad2, [=]() { _ChooseButton2(); });
	engine.GetInputHandler().RegisterButtonDownHandler(LLGL::Key::Return,
	                                                   [=, &engine, &registry]()
	                                                   {
		                                                   _ChooseButtonEnter();
		                                                   _InitGameplayEntities(engine, registry);
	                                                   });
}

void World::Update(const EngineContext& engine, EntityRegistry& registry)
{
	// continually update score
	mScoreManager->Update(engine, registry, mPlayer1->GetPlayerId(), mPlayer2->GetPlayerId());
	_UpdateCatFeatures(engine);

	if (mAIPlayerSystem)
	{
		mAIPlayerSystem->Update(registry);
	}
}

void World::Render(const EngineContext& engine, EntityRegistry& registry) const
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
		engine.DrawBox({-210, -350, 0.0f}, {200, 100, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, false);
	}
	else if (mInput2)
	{
		// highlight right box
		engine.DrawBox({290, -350, 0.0f}, {230, 100, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, false);
	}

	engine.DrawText2D("Press 1 or 2 to select", {320, 500}, {0.6, 0.6});
}

void World::_UpdateCatFeatures(const EngineContext& engine)
{
	// move eye pupils to follow mouse ball
	const auto& transform = engine.GetEntityRegistry().GetComponent<COGTransform>(mBall);
	const auto& pos = transform.GetPosition();
	if (pos.x < -10.0f)
	{
		// ball left of cat
		mEyePos1.x = -55.0f;
		mEyePos2.x = 35.0f;
	}
	if (pos.x > 10.0f)
	{
		// ball right
		mEyePos1.x = -35.0f;
		mEyePos2.x = 55.0f;
	}
	if (pos.y < 250.0f)
	{
		// ball up
		mEyePos1.y = 240.0f;
		mEyePos2.y = 240.0f;
	}
	if (pos.y > 300.0f)
	{
		// ball down
		mEyePos1.y = 260.0f;
		mEyePos2.y = 260.0f;
	}
	if (pos.x == 200.0f && pos.y == 200.0f)
	{
		mEyePos1 = {-40.0f, 250};
		mEyePos2 = {40.0f, 250};
	}

	// Update timer for cat mouth animation
	if (mScoreAnimationTimer > 0)
	{
		mScoreAnimationTimer--;
	}
}

void World::_InitGameplayEntities(const EngineContext& engine, EntityRegistry& registry)
{
	mPlayer1 = std::make_unique<Player>(registry, glm::vec2{-400, 100}, glm::vec4{1.0f, 229, 128, 1.0f},
	                                    glm::vec2{1, 800},
	                                    glm::vec2{400, -100}, glm::vec4{0, 0, 1.0f, 1.0f});

	mPlayer1->SetUserInputs(engine, registry, LLGL::Key::W, LLGL::Key::S);

	mPlayer2 = std::make_unique<Player>(registry, glm::vec2{378, 100}, glm::vec4{1.0f, 229, 128, 1.0f},
	                                    glm::vec2{1, 800},
	                                    glm::vec2{-405, -100}, glm::vec4{0, 0, 1.0f, 1.0f});

	mBall = _CreateBall(registry, {0, 0}, {179, 170, 154, 1.0f});
	mScoreManager = std::make_unique<ScoreManager>();

	if (mInput2)
	{
		SetPlayerAI(registry);
		mAIPlayerSystem = std::make_unique<AIPlayerSystem>(mPlayer2, mBall);
	}
	else
	{
		mPlayer2->SetUserInputs(engine, registry, LLGL::Key::Up, LLGL::Key::Down);
	}

	auto callback = [](COGPhysics& physics, COGPhysics& otherPhysics, int mask)
	{
		if (mask == static_cast<int>(CollisionMask::Paddle))
		{
			auto velocity = otherPhysics.GetVelocity();
			otherPhysics.SetVelocity({velocity.x, -velocity.y});
		}
	};

	int mask = static_cast<int>(CollisionMask::Paddle) | static_cast<int>(CollisionMask::Ball);
	// boundaries
	const auto boundary1 = registry.CreateEntity();
	registry.AddComponent<COGTransform>(boundary1, glm::vec2{-400, 550});
	registry.AddComponent<COGBoxShape>(boundary1, 800.0f, 40.0f, glm::vec4{0, 0, 0, 1});
	registry.AddComponent<COGPhysics>(boundary1, glm::vec2{0, 0}, glm::vec2(0, 1));
	registry.AddComponent<COGCollision>(boundary1, callback, static_cast<int>(CollisionMask::Boundary),
	                                    mask);

	const auto boundary2 = registry.CreateEntity();
	registry.AddComponent<COGTransform>(boundary2, glm::vec2{-400, -100});
	registry.AddComponent<COGBoxShape>(boundary2, 800.0f, 40.0f, glm::vec4{0, 0, 0, 1});
	registry.AddComponent<COGPhysics>(boundary2, glm::vec2{0, 0}, glm::vec2(0, 1));
	registry.AddComponent<COGCollision>(boundary2, callback, static_cast<int>(CollisionMask::Boundary),
	                                    mask);
}

EntityId World::_CreateBall(EntityRegistry& registry, glm::vec2 position, glm::vec4 color) const
{
	const auto ballEntity = registry.CreateEntity();
	registry.AddComponent<COGTransform>(ballEntity, position);
	registry.AddComponent<COGCircleShape>(ballEntity, fBallRadius, color);
	registry.AddComponent<COGPhysics>(ballEntity, glm::vec2{100, 70});
	registry.AddComponent<COGBounce>(ballEntity);

	return ballEntity;
}

void World::_DrawMouth(const EngineContext& engine) const
{
	//mouth
	if (mScoreAnimationTimer > 0)
	{
		// display surprised mouth for timer duration after player scores
		engine.DrawCircle({0.0f, 350.0f, 0.0f}, 0.2f, {255, 255, 255, 1.0f});
	}
	else
	{
		engine.DrawLine({0.0f, 340.0f, 0.0f}, {30.0f, 350.0f, 0.0f}, {255, 255, 255, 1.0f});
		engine.DrawLine({0.0f, 340.0f, 0.0f}, {-30.0f, 350.0f, 0.0f}, {255, 255, 255, 1.0f});
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

void World::_DrawKitty(const EngineContext& engine) const
{
	// head
	// color alpha 155
	engine.DrawCircle({0.0f, 300.0f, 0.0f}, 150.0f, {1.0f, 229, 0.5f, 0.5f});

	//nose
	engine.DrawCircle({-10.0f, 310.0f, 0.0f}, 8.0f, {1.0f, 0.5f, 0.8f, 1.0f});
	engine.DrawCircle({0.0f, 300.0f, 0.0f}, 10.0f, {1.0f, 0.5f, 0.8f, 1.0f});
	engine.DrawCircle({10.0f, 310.0f, 0.0f}, 8.0f, {1.0f, 0.5f, 0.8f, 1.0f});

	_DrawMouth(engine);
	_DrawEyes(engine);

	//ears 
	engine.DrawLine({-125.0, 220.0f, 0.0f}, {-95.0f, 100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawLine({-50.0, 160.0f, 0.0f}, {-95.0f, 100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawLine({125.0f, 220.0f, 0.0f}, {110.0f, 100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawLine({50.0, 160.0f, 0.0f}, {110.0f, 100.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});

	//whiskers left
	engine.DrawLine({-125.0, 310.0f, 0.0f}, {-10.0f, 320.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawLine({-125.0, 330.0f, 0.0f}, {-10.0f, 322.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawLine({-125.0, 345.0f, 0.0f}, {-10.0f, 325.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});

	// whiskers right
	engine.DrawLine({125.0, 310.0f, 0.0f}, {10.0f, 320.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawLine({125.0, 330.0f, 0.0f}, {10.0f, 322.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawLine({125.0, 345.0f, 0.0f}, {10.0f, 325.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f});
}


void World::_DrawEyes(const EngineContext& engine) const
{
	//eyes
	engine.DrawCircle({50.0f, 250.0f, 0.0f}, 30.0f, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawCircle({-50.0f, 250.0f, 0.0f}, 30.0f, {1.0f, 1.0f, 1.0f, 1.0f});
	engine.DrawCircle(glm::vec3(mEyePos1, 0.0f), 10.0f, {0.0f, 0.0f, 0.0f, 1.0f});
	engine.DrawCircle(glm::vec3(mEyePos2, 0.0f), 10.0f, {0.0f, 0.0f, 0.0f, 1.0f});
}
