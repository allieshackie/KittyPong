#pragma once

#include "Core/EventListener.h"
#include "Core/SceneManager.h"
#include "Core/SystemManager.h"
#include "Editor/EditorGUI.h"
#include "Entity/Descriptions/DescriptionFactory.h"
#include "Entity/Systems/COGCollisionSystem.h"
#include "Entity/Systems/COGPhysisSystem.h"
#include "Graphics/RenderContext.h"
#include "Graphics/Core/ResourceManager.h"
#include "Input/InputHandler.h"
#include "Map/MapRegistry.h"
#include "PongWorld.h"

class Game
{
public:
	Game(const LLGL::Extent2D screenSize = { 1200, 800 },
		const LLGL::UTF8String& title = "KittyPong",
		const LLGL::ColorRGBAf backgroundClearColor = { 0.0f, 0.0f, 0.0f, 1.0f },
		bool transparent = false);

	void Run();

	template <typename T>
	void RegisterDescription(const std::string& descriptionName) const
	{
		mDescriptionFactory->RegisterDescription<T>(descriptionName);
	}


private:
	std::unique_ptr<DescriptionFactory> mDescriptionFactory;
	std::unique_ptr<RenderContext> mRenderContext;
	std::unique_ptr<ResourceManager> mResourceManager;

	std::unique_ptr<SceneManager> mSceneManager;
	std::shared_ptr<GUISystem> mGUISystem;

	std::unique_ptr<EventPublisher> mEventPublisher;
	std::unique_ptr<SystemManager> mSystemManager;
	std::unique_ptr<MapRegistry> mMapRegistry;

	// systems
	std::shared_ptr<InputHandler> mInputHandler;
	std::shared_ptr<RenderSystem> mRenderSystem;

	bool mRunning = true;

	std::unique_ptr<COGPhysicsSystem> mPhysicsSystem;
	std::unique_ptr<COGCollisionSystem> mCollisionSystem;
	std::unique_ptr<PongWorld> mPongWorld;
};
