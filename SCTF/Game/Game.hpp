#pragma once

#include <filesystem>
#include <iostream>
#include <limits>

#include <SFML/Graphics.hpp>

#include "Configuration.hpp"
#include "Particles.hpp"
#include "Partitioner.hpp"
#include "VerletIntegrator.hpp"
#include "CollisionSolver.hpp"
#include "Border.hpp"
#include "FixPointConstraints.hpp"
#include "RigidConstraints.hpp"
#include "ElasticConstraints.hpp"
#include "Renderer.hpp"


class Game {
public:
	Game(Configuration& config) :
		config{ config },
		particles{config},
		border{config, particles},
		partitioner{config, border, particles, 100, 100},
		verletIntegrator{config, particles},
		collisionSolver{config, partitioner, particles},
		fixPointConstraint{config, particles},
		rigidConstraint{config, particles},
		elasticConstraint{config, particles},
		renderer{config, particles},
		time_to_simulate{0}
	{
		/*for (float i = 0; i < 100; i += 1)
		{
			for (float j = 0; j < 10; j += 1)
			{
				particles.AddParticle({ i, j });
			}
		}*/
	}

	void LoadMap(std::filesystem::path map_directory_relative)
	{
		std::filesystem::path map_directory = std::filesystem::current_path() / map_directory_relative;
		for (const auto& entry : std::filesystem::directory_iterator(map_directory))
		{
			std::cout << entry.path() << std::endl;

			sf::Image image;
			image.loadFromFile(entry.path().string().c_str());
			unsigned int width = image.getSize().x;
			unsigned int height = image.getSize().y;

			size_t no_id = std::numeric_limits<int>::max();
			std::vector<size_t> inserted_id(width * height, no_id);


			for (size_t i = 0; i < width * height; i++)
			{
				const size_t x = i % width;
				const size_t y = i / width;
				if (image.getPixel(x, y).a != 0)
				{
					size_t id = particles.AddParticle(sf::Vector2f(x + 0.5f, y + 0.5f), image.getPixel(x, y));
					inserted_id[i] = id;
				}
			}
			for (size_t i = 0; i < width * height; i++)
			{
				const size_t x = i % width;
				const size_t y = i / width;
				if (inserted_id[i] != no_id)
				{
					if (x < width - 1 && inserted_id[i + 1] != no_id)
					{
						rigidConstraint.AddConstraint(inserted_id[i], inserted_id[i + 1]);
					}
					if (y < height - 1 && inserted_id[i + width] != no_id)
					{
						rigidConstraint.AddConstraint(inserted_id[i], inserted_id[i + width]);
					}
				}
			}
		}
	}

	void Update(float dt)
	{
		time_to_simulate += dt;
		// TODO: Actually use dt;
		//while (time_to_simulate > 1.0f / config.framerate)
		{
			constexpr size_t n = 8;
			float time_sub_step = 1.0f / config.framerate / n;
			for (size_t i = 0; i < n; i++)
			{
				config.current_time += 1.0f / config.framerate / n;
				verletIntegrator.Update(time_sub_step);
				rigidConstraint.Update(time_sub_step);
				border.Update(time_sub_step);
				collisionSolver.Update(time_sub_step);
			}
			time_to_simulate -= 1.0f / config.framerate;
		}
	}
	
	void Draw(sf::RenderWindow& window)
	{
		renderer.Draw(window);
	}

private:
	Configuration& config;
	Particles particles;
	PrecalculatedRandomNumbers randomNumbers;
	Border border;
	Partitioner partitioner;
	VerletIntegrator verletIntegrator;
	CollisionSolver collisionSolver;
	FixPointConstraints fixPointConstraint;
	RigidConstraints rigidConstraint;
	ElasticConstraints elasticConstraint;
	Renderer renderer;
	float time_to_simulate;
};
