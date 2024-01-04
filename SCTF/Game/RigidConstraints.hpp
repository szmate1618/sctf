#pragma once

#include <vector>

#include "Configuration.hpp"
#include "Particles.hpp"


class RigidConstraints {
public:
	RigidConstraints(const Configuration& config, Particles& particles) : config{ config }, particles{ particles } {}

	void AddConstraint(size_t id1, size_t id2, float length)
	{
		constraints.push_back({ id1, id2});
		lengths.push_back(length);
	}
	
	void Update(float dt)
	{
		for (size_t i = 0; i < constraints.size(); i++)
		{
			size_t id1 = constraints[i].first;
			size_t id2 = constraints[i].second;
			float length = lengths[i];

			//TODO: Handle different lengths, not just unit.
			sf::Vector2f collision_vector = particles.current_position[id1] - particles.current_position[id2];

			float distance = GetLength(collision_vector);
			if (distance == 0) continue;
			if (distance > 0.999f * length && distance < 1.1f * length) continue;
			sf::Vector2f normalized_collision_vector = collision_vector / distance;
			particles.current_position[id1] = particles.current_position[id1] + normalized_collision_vector * (length - distance) / 2.0f;
			particles.current_position[id2] = particles.current_position[id2] - normalized_collision_vector * (length - distance) / 2.0f;
		}
	}

private:
	const Configuration& config;
	Particles& particles;

	std::vector<std::pair<size_t, size_t>> constraints;
	std::vector<float> lengths;
};
