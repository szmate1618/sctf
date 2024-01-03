#pragma once

#include <vector>

#include "Configuration.hpp"
#include "Particles.hpp"


class RigidConstraints {
public:
	RigidConstraints(const Configuration& config, Particles& particles) : config{ config }, particles{ particles } {}

	void AddConstraint(size_t id1, size_t id2)
	{
		constraint.push_back({ id1, id2 });
	}
	
	void Update(float dt)
	{
		for (const auto& c : constraint)
		{
			size_t id1 = c.first;
			size_t id2 = c.second;

			//TODO: Handle different lengths, not just unit.
			sf::Vector2f collision_vector = particles.current_position[id1] - particles.current_position[id2];

			float distance_squared = GetLengthSquared(collision_vector);
			if (distance_squared > 1)
			{
				float distance = GetLength(collision_vector);
				if (distance == 0) continue;
				sf::Vector2f normalized_collision_vector = collision_vector / distance;
				particles.current_position[id1] += normalized_collision_vector * (1 - distance) / 2.0f;
				particles.current_position[id2] -= normalized_collision_vector * (1 - distance) / 2.0f;
			}
		}
	}

private:
	const Configuration& config;
	Particles& particles;

	std::vector<std::pair<size_t, size_t>> constraint;
};
