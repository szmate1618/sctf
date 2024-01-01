#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include "Configuration.hpp"
#include "MaterialType.hpp"


struct Particles
{
	Particles(const Configuration& config) : config{ config } {}

	size_t GetNumberOfParticles() const { return current_position.size(); };

	size_t AddParticle(sf::Vector2f position, sf::Color color = sf::Color::Green, MaterialType type = MaterialType::Basic)
	{
		current_position.push_back(position);
		previous_position.push_back(position);
		acceleration.push_back({ 0,0 });
		this->color.push_back(color);
		material_type.push_back(type);
		is_destroyed.push_back(false);

		return current_position.size() - 1;
	}

	const Configuration& config;

	std::vector<sf::Vector2f> current_position;
	std::vector<sf::Vector2f> previous_position;
	std::vector<sf::Vector2f> acceleration;
	std::vector<sf::Color> color;
	std::vector<MaterialType> material_type;
	std::vector<bool> is_destroyed;

};
