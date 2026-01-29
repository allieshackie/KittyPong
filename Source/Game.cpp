#include "Core/Timer.h"
#include "Entity/Components/ModelComponent.h"
#include "Entity/Descriptions/AnimationDescription.h"
#include "Entity/Descriptions/CollisionDescription.h"
#include "Entity/Descriptions/LightDescription.h"
#include "Entity/Descriptions/ModelDescription.h"
#include "Entity/Descriptions/PhysicsDescription.h"
#include "Entity/Descriptions/PlayerDescription.h"
#include "Entity/Descriptions/ScriptDescription.h"
#include "Entity/Descriptions/SpriteDescription.h"
#include "Entity/Descriptions/TransformDescription.h"
#include "Graphics/Animation/Animator.h"
#include "Graphics/RenderSystem.h"
#include "GUI/GUISystem.h"
#include "GUI/GUIMenu.h"
#include "Physics/PhysicsSystem.h"
#include "Script/LuaSystem.h"

#include "Game.h"

Game::Game(const LLGL::Extent2D screenSize, const LLGL::UTF8String& title,
	const LLGL::ColorRGBAf backgroundClearColor, bool transparent)
{
	// Init all systems **without** dependencies
	mSystemManager = std::make_unique<SystemManager>();
	mInputHandler = std::make_shared<InputHandler>();
	mResourceManager = std::make_unique<ResourceManager>();
	mDescriptionFactory = std::make_unique<DescriptionFactory>();
	mEventPublisher = std::make_unique<EventPublisher>();
	mMapRegistry = std::make_unique<MapRegistry>();

	// Must be called before SceneManager sets up description factory
	mDescriptionFactory->RegisterDescription<CollisionDescription>(CollisionDescription::JsonName);
	mDescriptionFactory->RegisterDescription<PhysicsDescription>(PhysicsDescription::JsonName);
	mDescriptionFactory->RegisterDescription<PlayerDescription>(PlayerDescription::JsonName);
	mDescriptionFactory->RegisterDescription<ScriptDescription>(ScriptDescription::JsonName);
	mDescriptionFactory->RegisterDescription<SpriteDescription>(SpriteDescription::JsonName);
	mDescriptionFactory->RegisterDescription<TransformDescription>(TransformDescription::JsonName);

	// Init all systems **with** dependencies
	mRenderContext = std::make_unique<RenderContext>(screenSize, backgroundClearColor, title, mInputHandler,
		transparent);
	mGUISystem = std::make_shared<GUISystem>(*mRenderContext);
	mSceneManager = std::make_unique<SceneManager>(*mDescriptionFactory, *mMapRegistry, *mResourceManager);

	// Remaining system setup
	mInputHandler->SetGUISystem(mGUISystem);
	mInputHandler->RegisterButtonUpHandler(LLGL::Key::Escape, [=]() { mRunning = false; });
	mResourceManager->LoadAllResources(mRenderContext->GetRenderSystem());

	auto world = mSceneManager->GetCurrentWorld();
	mRenderSystem = mSystemManager->RegisterSystem<RenderSystem>(*mRenderContext, *mResourceManager, world);
	// Gameplay systems
	mSystemManager->RegisterSystem<EditorGUI>(mInputHandler, world, *mRenderContext, *mResourceManager, *mRenderSystem);
	mSystemManager->RegisterSystem<COGPhysicsSystem>(world);
	mSystemManager->RegisterSystem<COGCollisionSystem>(world);

	// Need to register AIPlayerSystem if 1 vs AI choice

	mPongWorld = std::make_unique<PongWorld>(mInputHandler, world, *mRenderSystem);
}

void Game::Run()
{
	//mRenderContext->SetBackgroundClearColor({ 0.1f, 0.1f, 0.1f });
	mRenderSystem->LoadFont("afternight.ttf");
	mSceneManager->LoadScene("kittypong.json");

	// Init current time
	auto timer = new Timer();
	timer->mCurrentTime = Clock::now();

	double frames = 0;
	double fps = 0;
	double fpsTimer = 0.0;

	while (mRenderContext->ProcessEvents() && mRunning)
	{
		auto newTime = Clock::now();
		Duration frameDuration = newTime - timer->mCurrentTime;
		timer->mCurrentTime = newTime;

		double deltaTime = frameDuration.count();
		if (deltaTime > 0.25)
		{
			// Clamp large deltaTime to prevent "spiral of death"
			// where we could potentially always be "catching up"
			deltaTime = 0.25;
		}

		fpsTimer += deltaTime;
		frames++;

		if (fpsTimer >= 1.0)
		{
			fps = frames / fpsTimer;
			frames = 0.0;
			fpsTimer = 0.0;
		}

		timer->mAccumulator += deltaTime;
		if (timer->mAccumulator > timer->mAccumulatorMax)
		{
			timer->mAccumulator = timer->mAccumulatorMax;
		}

		while (timer->mAccumulator >= timer->mDT)
		{
			mSystemManager->FixedUpdate(timer->mDT);
			timer->mAccumulator -= timer->mDT;
		}

		mInputHandler->Update();
		mSystemManager->Update(deltaTime);
		mPongWorld->Update();

		auto world = mSceneManager->GetCurrentWorld();
		if (world)
		{
			world->GetCamera().Update();

			mRenderContext->BeginFrame();

			mSystemManager->Render(world);
			mPongWorld->Render();

			mRenderContext->EndFrame();
		}
	}
}
