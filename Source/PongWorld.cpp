#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "LLGL/Key.h"
#include "Input/InputHandler.h"
#include "Graphics/RenderSystem.h"

#include "Core/World.h"
#include "Entity/Player.h"
#include "Entity/Components/AIPlayer.h"
#include "Entity/Components/COGBounce.h"
#include "Entity/Components/COGBoxShape.h"
#include "Entity/Components/COGCircleShape.h"
#include "Entity/Components/COGCollision.h"
#include "Entity/Components/COGPhysics.h"
#include "Entity/Components/COGTransform.h"
#include "Entity/Systems/ScoreManager.h"

#include "PongWorld.h"

PongWorld::PongWorld(std::weak_ptr<InputHandler> inputHandler, std::weak_ptr<World> world, RenderSystem& renderSystem) 
	: mRenderSystem(renderSystem), mWorld(world)
{
	if (auto inputHandlerPtr = inputHandler.lock())
	{

		inputHandlerPtr->RegisterButtonDownHandler(LLGL::Key::Keypad1, [=]() { _ChooseButton1(); });
		inputHandlerPtr->RegisterButtonDownHandler(LLGL::Key::Keypad2, [=]() { _ChooseButton2(); });
		inputHandlerPtr->RegisterButtonDownHandler(LLGL::Key::Return,
	                                                   [=]()
	                                                   {
		                                                   _ChooseButtonEnter();
		                                                   _InitGameplayEntities(inputHandler);
	                                                   });
	}
}

void PongWorld::Update()
{
	if (!mHasGameStarted)
	{
		return;
	}
	// continually update score
	mScoreManager->Update(mRenderSystem, mWorld, mPlayer1->GetPlayerId(), mPlayer2->GetPlayerId());
	_UpdateCatFeatures();

	if (mAIPlayerSystem)
	{
		mAIPlayerSystem->Update(mWorld);
	}
}

void PongWorld::Render() const
{
	mRenderSystem.DrawTextFont("KITTY PONG", { -1.5, 3.5 }, { 0.01, -0.01 }, { 100, 0, 0, 255 }); // display header
	_DrawKitty();
	if (!mHasGameStarted)
	{
		_Menu();
	}
	else
	{
		if (auto worldPtr = mWorld.lock())
		{
			const auto circleView = worldPtr->GetRegistry().view<COGCircleShape, COGTransform>();
			circleView.each([=](auto& shape, auto& transform)
			{
				shape.Render(mRenderSystem, transform);
			});
			const auto boxView = worldPtr->GetRegistry().view<COGBoxShape, COGTransform>();
			boxView.each([=](auto& shape, auto& transform)
			{
				shape.Render(mRenderSystem, transform);
			});
		}

	}
}

void PongWorld::SetPlayerAI() const
{
	if (auto worldPtr = mWorld.lock())
	{
		Entity* player2 = worldPtr->GetEntityForId(mPlayer2->GetPlayerId());
		player2->AddComponentWithArgs<AIPlayer>();
	}
}

void PongWorld::_Menu() const
{
	mRenderSystem.DrawTextFont("KITTY PONG", {200, 10}, {2, 2}, {1,1,1,1});

	// buttons
	mRenderSystem.DrawBox({-3, -0.5, 1.5f}, {2, 1, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, mInput1);
	mRenderSystem.DrawTextFont("1 VS 1", { -3, 0 }, {0.01, -0.01}, {1,0,0,1});

	mRenderSystem.DrawBox({1, -0.5, 1.5f}, {2, 1, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, mInput2);
	mRenderSystem.DrawTextFont("1 VS AI", { 1.5, 0 }, {0.01, -0.01}, {0,1,0,1});

	mRenderSystem.DrawTextFont("Press 1 or 2 to select", {320, 500}, {0.6, 0.6}, {1,1,1,1});
}

void PongWorld::_UpdateCatFeatures()
{
	if (auto worldPtr = mWorld.lock())
	{
		Entity* ball = worldPtr->GetEntityForId(mBall);
		// move eye pupils to follow mouse ball
		const auto& transform = ball->GetComponent<COGTransform>();
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
	}

	// Update timer for cat mouth animation
	if (mScoreAnimationTimer > 0)
	{
		mScoreAnimationTimer--;
	}
}

void PongWorld::_InitGameplayEntities(std::weak_ptr<InputHandler> inputHandler)
{
	mPlayer1 = std::make_unique<Player>(mWorld, glm::vec2{-400, 100}, glm::vec4{1.0f, 229, 128, 1.0f},
	                                    glm::vec2{1, 800},
	                                    glm::vec2{400, -100}, glm::vec4{0, 0, 1.0f, 1.0f});

	mPlayer1->SetUserInputs(inputHandler, LLGL::Key::W, LLGL::Key::S);

	mPlayer2 = std::make_unique<Player>(mWorld, glm::vec2{378, 100}, glm::vec4{1.0f, 229, 128, 1.0f},
	                                    glm::vec2{1, 800},
	                                    glm::vec2{-405, -100}, glm::vec4{0, 0, 1.0f, 1.0f});

	_CreateBall({0, 0}, {179, 170, 154, 1.0f});
	mScoreManager = std::make_unique<ScoreManager>();

	if (mInput2)
	{
		SetPlayerAI();
		mAIPlayerSystem = std::make_unique<AIPlayerSystem>(mPlayer2, mBall);
	}
	else
	{
		mPlayer2->SetUserInputs(inputHandler, LLGL::Key::Up, LLGL::Key::Down);
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

	if (auto worldPtr = mWorld.lock())
	{
		// boundaries
		Entity& boundary1 = worldPtr->CreateEntity();
		boundary1.AddComponentWithArgs<COGTransform>(glm::vec2{-400, 550});
		boundary1.AddComponentWithArgs<COGBoxShape>(800.0f, 40.0f, glm::vec4{0, 0, 0, 1});
		boundary1.AddComponentWithArgs<COGPhysics>(glm::vec2{0, 0}, glm::vec2(0, 1));
		boundary1.AddComponentWithArgs<COGCollision>(callback, static_cast<int>(CollisionMask::Boundary), mask);

		Entity& boundary2 = worldPtr->CreateEntity();
		boundary2.AddComponentWithArgs<COGTransform>(glm::vec2{-400, -100});
		boundary2.AddComponentWithArgs<COGBoxShape>(800.0f, 40.0f, glm::vec4{0, 0, 0, 1});
		boundary2.AddComponentWithArgs<COGPhysics>(glm::vec2{0, 0}, glm::vec2(0, 1));
		boundary2.AddComponentWithArgs<COGCollision>(callback, static_cast<int>(CollisionMask::Boundary), mask);
	}
}

void PongWorld::_CreateBall(glm::vec2 position, glm::vec4 color)
{
	if (auto worldPtr = mWorld.lock())
	{
		Entity& ballEntity = worldPtr->CreateEntity();
		ballEntity.AddComponentWithArgs<COGTransform>(position);
		ballEntity.AddComponentWithArgs<COGCircleShape>(fBallRadius, color);
		ballEntity.AddComponentWithArgs<COGPhysics>(glm::vec2{100, 70});
		ballEntity.AddComponentWithArgs<COGBounce>();

		mBall = ballEntity.GetId();
	}
}

void PongWorld::_ChooseButton1()
{
	mInput1 = true;
	mInput2 = false;
}

void PongWorld::_ChooseButton2()
{
	mInput1 = false;
	mInput2 = true;
}

void PongWorld::_ChooseButtonEnter()
{
	if (mInput1 || mInput2)
	{
		mHasGameStarted = true;
	}
}

void PongWorld::_DrawKitty() const
{
	// head
	// color alpha 155
	mRenderSystem.DrawCircle({ 0, 0, 1.5f }, 2.0f, {1.0f, 229, 0.5f, 0.5f});

	//nose
	mRenderSystem.DrawCircle({ -0.16, -0.35, 1.5f }, 0.1f, {1.0f, 0.5f, 0.8f, 1.0f});
	mRenderSystem.DrawCircle({ 0, -0.45, 1.5f }, 0.15f, {1.0f, 0.5f, 0.8f, 1.0f});
	mRenderSystem.DrawCircle({ 0.16, -0.35, 1.5f }, 0.1f, {1.0f, 0.5f, 0.8f, 1.0f});

	//mouth
	if (mScoreAnimationTimer > 0)
	{
		// display surprised mouth for timer duration after player scores
		mRenderSystem.DrawCircle({ 0, -0.8, 1.5f }, 0.2f, { 0.0f, 0.0f, 0.0f, 1.0f });
	}
	else
	{
		mRenderSystem.DrawLine({ -0.4, -0.8, 1.5f }, { 0, -0.6, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f });
		mRenderSystem.DrawLine({ 0.4, -0.8, 1.5f }, { 0, -0.6, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f });
	}

	//eyes
	mRenderSystem.DrawCircle({ -0.7, 0.25, 1.5f }, 0.35f, { 1.0f, 1.0f, 1.0f, 1.0f });
	mRenderSystem.DrawCircle({ 0.7, 0.25, 1.5f }, 0.35f, { 1.0f, 1.0f, 1.0f, 1.0f });
	mRenderSystem.DrawCircle(glm::vec3(mEyePos1, 1.5f), 0.15f, { 0.0f, 0.0f, 0.0f, 1.0f });
	mRenderSystem.DrawCircle(glm::vec3(mEyePos2, 1.5f), 0.15f, { 0.0f, 0.0f, 0.0f, 1.0f });

	//ears 
	mRenderSystem.DrawLine({ -1.5, 1.3, 1.5f }, { -1.2, 2.8, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f});
	mRenderSystem.DrawLine({ -0.4, 1.9, 1.5f }, { -1.2, 2.8, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f});

	mRenderSystem.DrawLine({ 0.4, 1.9, 1.5f }, { 1.2, 2.8, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f });
	mRenderSystem.DrawLine({ 1.5, 1.3, 1.5f }, { 1.2, 2.8, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f });

	//whiskers left
	mRenderSystem.DrawLine({ -1.5, -0.2, 1.5f }, { -0.4, -0.35, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f});
	mRenderSystem.DrawLine({ -1.8, -0.5, 1.5f }, { -0.45, -0.4, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f});
	mRenderSystem.DrawLine({ -1.6, -0.9, 1.5f }, { -0.4, -0.45, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f});

	// whiskers right
	mRenderSystem.DrawLine({ 1.5, -0.2, 1.5f }, { 0.4, -0.35, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f });
	mRenderSystem.DrawLine({ 1.8, -0.5, 1.5f }, { 0.45, -0.4, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f });
	mRenderSystem.DrawLine({ 1.6, -0.9, 1.5f }, { 0.4, -0.45, 1.5f }, { 0.0f, 0.0f, 0.0f, 1.0f });
}
