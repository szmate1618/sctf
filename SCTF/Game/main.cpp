#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Configuration.hpp"
#include "Game.hpp"
#include "Camera.hpp"

int main()
{
	Configuration config{ 60,101,101,0.0 };
	Game game{ config };

	game.LoadMap("map001");
	//game.AddSomeBalls();

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "M�t� a kir�ly");
	window.setFramerateLimit(config.framerate);
	sf::Clock clock;

	Camera camera(config, window);
	camera.Zoom(0.1f);

	int mouse_counter = 0;

	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			switch (event.type)
			{
				case sf::Event::Closed:
				{
					window.close();
					break;
				}
				case sf::Event::Resized:
				{
					camera.Resize(event.size.width, event.size.height);
					break;
				}
				case sf::Event::MouseWheelScrolled:
				{
					camera.SetZoom(camera.GetZoom() + event.mouseWheelScroll.delta / 100.0f);
					break;
				}
			}
		}

		/*if (mouse_counter > 0) mouse_counter--;
		if (mouse_counter == 0 && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i localPosition = sf::Mouse::getPosition(window);
			sf::Vector2f worldPosition = window.mapPixelToCoords(localPosition);
			sf::Vector2f position{ worldPosition.x, worldPosition.y };
			particles.current_position.push_back(position);
			particles.previous_position.push_back(position);
			particles.acceleration.push_back({ 0, 0 });

			mouse_counter = 2;
		}*/

		sf::Time elapsed = clock.restart();

		sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
		float delta_min_x = mouse_position.x - window.getSize().x * 0.15;
		float delta_max_x = window.getSize().x * 0.85 - mouse_position.x;
		float delta_min_y = mouse_position.y - window.getSize().y * 0.15;
		float delta_max_y = window.getSize().y * 0.85 - mouse_position.y;
		if (delta_min_x < 0) camera.Move(delta_min_x * elapsed.asSeconds() * 0.5f, 0);
		if (delta_max_x < 0) camera.Move(-delta_max_x * elapsed.asSeconds() * 0.5f, 0);
		if (delta_min_y < 0) camera.Move(0.0f, delta_min_y * elapsed.asSeconds() * 0.5f);
		if (delta_max_y < 0) camera.Move(0.0f, -delta_max_y * elapsed.asSeconds() * 0.5f);
		
		std::cout << elapsed.asSeconds() << std::endl;
		game.Update(elapsed.asSeconds());

		//window.setView(view);
		// clear the window with black color
		window.clear(sf::Color::Black);

		game.Draw(window);

		// end the current frame (internally swaps the front and back buffers)
		window.display();
	}

	return 0;
}
