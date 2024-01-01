#pragma once

#include "Configuration.hpp"
#include "Particles.hpp"
#include "Math.hpp"


class VerletIntegrator {
public:
	VerletIntegrator(const Configuration& config, Particles& particles) : config{ config }, particles{ particles } {}
	void Update(float dt)
	{
		for (size_t id = 0; id < particles.GetNumberOfParticles(); id++)
		{
			const auto old_position = particles.current_position[id];
			const auto velocityXdt = (particles.current_position[id] - particles.previous_position[id]);
			if (GetLength(velocityXdt / dt) < 0.6) particles.previous_position[id] = particles.current_position[id] - velocityXdt * 0.8f;
			particles.current_position[id] = particles.current_position[id] * 2.0f - particles.previous_position[id] + (particles.acceleration[id]) * dt * dt - velocityXdt * GetLength(velocityXdt)*10.0f*dt;
			particles.previous_position[id] = old_position;
			particles.acceleration[id] = { 0, 98.1f };
		}
	}

private:
	const Configuration& config;
	Particles& particles;
};
