#include "Core/EngineContext.h"
#include "Game.h"

int main()
{
	const auto game = new Game();
	const auto engine = std::make_unique<EngineContext>(game, LLGL::Extent2D{800, 600}, "Kitty Pong",
	                                                    LLGL::ColorRGBAf{0.1f, 0.1f, 0.1f, 1.0f}, false);

	engine->Run(game);

	return 0;
}
