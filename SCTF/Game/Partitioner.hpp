#pragma once

#include <vector>

#include "Configuration.hpp"
#include "Partition.hpp"
#include "Border.hpp"
#include "Particles.hpp"


class Partitioner {
public:
	Partitioner(const Configuration& config, const Border& border, Particles& particles, size_t number_of_columns, size_t number_of_rows) :
		config{ config },
		border{ border },
		particles{ particles },
		number_of_columns{ number_of_columns },
		number_of_rows{ number_of_rows }
	{
		// Column-first indexing, because maybe traversing by columns would be faster,
		// because of how things pile up vertically because of gravity.
		partitions.resize(number_of_columns);
		for (auto& column : partitions)
		{
			column.resize(number_of_rows);
			for (auto& partition : column)
			{
				partition.ids.reserve(320); //TODO: MAke this configurable. Also maybe do this in the partition constructor.
			}
		}
	}

	void MaybeInsertIntoPartitionWithOffset(size_t index, float offset_x, float offset_y)
	{
		sf::Vector2f p = particles.current_position[index];
		size_t x = static_cast<size_t>((p.x + offset_x) / (config.width / number_of_columns));
		size_t y = static_cast<size_t>((p.y + offset_y) / (config.height / number_of_rows));
		if (x >= number_of_columns) x = number_of_columns - 1;
		if (y >= number_of_rows) y = number_of_rows - 1;

		//if (partitions[x][y].ids.size() < 320)
		{
			if (partitions[x][y].ids.empty() || partitions[x][y].ids.back() != index)
			{
				partitions[x][y].ids.push_back(index);
			}
		}
	}

	void MaybeInsertIntoPartition(size_t index)
	{
		MaybeInsertIntoPartitionWithOffset(index, 0.5, 0.5);
		MaybeInsertIntoPartitionWithOffset(index, -0.5, 0.5);
		MaybeInsertIntoPartitionWithOffset(index, -0.5, -0.5);
		MaybeInsertIntoPartitionWithOffset(index, 0.5, -0.5);
	}

//private: //TODO: Do this correctly.
	size_t number_of_columns;
	size_t number_of_rows;
	const Configuration& config;
	const Border& border;
	Particles& particles;
	std::vector<std::vector<Partition>> partitions;
};
