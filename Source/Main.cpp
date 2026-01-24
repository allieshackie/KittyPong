#include "Game.h"

int main()
{
	const auto game = std::make_unique<Game>(LLGL::Extent2D{ 1200, 800 }, "Game",
		LLGL::ColorRGBAf{ 0.2f, 0.5f, 1.0f, 1.0f });

	game->Run();

	return 0;
}
