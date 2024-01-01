#pragma once

#include <vector>
#include "Configuration.hpp"


struct Partition {
	Partition() {};
	Partition(float min_x, float max_x, float min_y, float max_y) {};

	size_t GetNumberOfParticles() const
	{
		return ids.size();
	}

	float getMinX() const { return min_x; };
	float getMaxX() const { return max_x; };
	float getMinY() const { return min_y; };
	float getMaxY() const { return max_y; };

	float min_x;
	float max_x;
	float min_y;
	float max_y;

	std::vector<size_t> ids;
};
