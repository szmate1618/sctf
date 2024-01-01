#pragma once

#include "Configuration.hpp"
#include "Particles.hpp"
#include "PrecalculatedRandomNumbers.hpp"


class Border {
public:
	Border(const Configuration& config, Particles& particles):
		config{ config },
		particles{ particles }
	{}

	void Update(float dt)
	{
		for (size_t id = 0; id < particles.GetNumberOfParticles(); id++)
		{
			if (particles.current_position[id].x - 0.5f < 0) particles.current_position[id].x = 0 + 0.5f + random_numbers.GetNext() / 10;
			if (particles.current_position[id].x + 0.5f > config.width ) particles.current_position[id].x = config.width - 0.5f - random_numbers.GetNext() / 10;
			if (particles.current_position[id].y - 0.5f < 0) particles.current_position[id].y = 0 + 0.5f + random_numbers.GetNext() / 10;
			if (particles.current_position[id].y + 0.5f > config.height) particles.current_position[id].y = config.height - 0.5f - random_numbers.GetNext() / 10;
		}
	}

private:
	const Configuration& config;
	Particles& particles;
	PrecalculatedRandomNumbers random_numbers;
};
