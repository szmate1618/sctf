#include <iostream>
#include <chrono>

#include "../Game/Configuration.hpp"
#include "../Game/Game.hpp"
#include "../Game/Camera.hpp"


int main()
{
	Configuration config{ 60,101,101,0.0 };
	Game game{ config };

	game.LoadMap("../Game/map001");

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (size_t i = 0; i < config.framerate * 120; i++)
	{
		game.Update(1.f/config.framerate);
	}
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << " seconds" << std::endl;

	return 0;
}
