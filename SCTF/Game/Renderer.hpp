#pragma once

#include <SFML/Graphics.hpp>

#include "Configuration.hpp"
#include "Particles.hpp"


class Renderer {
public:
	Renderer(const Configuration& config, const Particles& particles) :
		config{ config },
		particles{ particles }
	{

	}

	void Draw(sf::RenderWindow& window)
	{
		sf::VertexArray vertices(sf::Quads, 4 * particles.GetNumberOfParticles()); // Actually sf::Quads is deprecated, but whatever.
		for (size_t id = 0; id < particles.GetNumberOfParticles(); id++)
		{
			vertices[4 * id + 0].position = particles.current_position[id] + sf::Vector2f(0.5, -0.5);
			vertices[4 * id + 1].position = particles.current_position[id] + sf::Vector2f(-0.5, -0.5);
			vertices[4 * id + 2].position = particles.current_position[id] + sf::Vector2f(-0.5, 0.5);
			vertices[4 * id + 3].position = particles.current_position[id] + sf::Vector2f(0.5, 0.5);

			vertices[4 * id + 0].color = particles.color[id];
			vertices[4 * id + 1].color = particles.color[id];
			vertices[4 * id + 2].color = particles.color[id];
			vertices[4 * id + 3].color = particles.color[id];
		}
		window.draw(vertices);
	}

private:
	const Configuration& config;
	const Particles& particles;
};
