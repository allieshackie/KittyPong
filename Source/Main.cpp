#include "Core/EngineContext.h"
#include "Game.h"

int main()
{
	const auto engine = std::make_unique<EngineContext>();
	const auto game = new Game();

	engine->Run(game);

	return 0;
}
