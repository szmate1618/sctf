#pragma once

#include <thread>
#include <atomic>
#include <barrier>

#include "Particles.hpp"
#include "Partitioner.hpp"


#define parallelism 8
std::barrier phase1_start_barrier(parallelism + 1);
std::barrier phase1_end_barrier(parallelism + 1);
std::barrier phase2_start_barrier(parallelism + 1);
std::barrier phase2_end_barrier(parallelism + 1);

class CollisionSolver
{
public:
	CollisionSolver(const Configuration& config, Partitioner& partitioner, Particles& particles):
		config{ config },
		particles{ particles },
		partitioner{ partitioner },
		running{ true }
	{
		std::vector<size_t> column_indices[2 * parallelism];
		size_t columns_per_thread = partitioner.number_of_columns / parallelism;
		for (size_t i = 0; i < columns_per_thread - 1; i++)
		{
			for (size_t j = 0; j < parallelism; j++)
			{
				column_indices[j].push_back(j * columns_per_thread + i);
			}
		}
		for (size_t j = 0; j < parallelism; j++)
		{
			column_indices[4 + j].push_back((j + 1) * columns_per_thread - 1);
		}

		for (size_t i = 0; i < parallelism; i++)
		{
			phase1_threads.emplace_back([this, column_indices, i]() { while (running) { UpdateColumns(column_indices[i], i); } });
			phase2_threads.emplace_back([this, column_indices, i]() { while (running) { UpdateColumns(column_indices[parallelism + i], parallelism + i); } });
		}
	}

	~CollisionSolver()
	{
		running = false;
		phase1_start_barrier.arrive_and_drop();
		phase2_start_barrier.arrive_and_drop();
		for (size_t i = 0; i < parallelism; i++)
		{
			phase1_threads[i].join();
		}
		for (size_t i = 0; i < parallelism; i++)
		{
			phase2_threads[i].join();
		}
	}

	void UpdateColumns(const std::vector<size_t>& column_indices, size_t thread_id)
	{
		if (thread_id < parallelism) { phase1_start_barrier.arrive_and_wait(); }
		else { phase2_start_barrier.arrive_and_wait(); }
		if (!running) return;

		for (size_t i: column_indices)
		{
			for (size_t j = 0; j < partitioner.number_of_rows; j++)
			{
				if (partitioner.partitions[i][j].ids.size() >= 2)
				{
					for (size_t k = 0; k < partitioner.partitions[i][j].ids.size() - 1; k++)
					{
						for (size_t l = k + 1; l < partitioner.partitions[i][j].ids.size(); l++)
						{
							const auto& id1 = partitioner.partitions[i][j].ids[k];
							const auto& id2 = partitioner.partitions[i][j].ids[l];
							sf::Vector2f collision_vector = particles.current_position[id1] - particles.current_position[id2]; //TODO: Handle null collision vector.

							if (collision_vector.x == 0 && collision_vector.y == 0)
							{
								continue;
							}

							float distance_squared = GetLengthSquared(collision_vector);
							if (distance_squared < 1)
							{
								float distance = GetLengthForUnitOrShorter(collision_vector);
								if (distance == 0) continue;
								sf::Vector2f normalized_collision_vector = collision_vector / distance;
								particles.current_position[id1] = particles.current_position[id1] + normalized_collision_vector * (1 - distance) / 2.0f;
								particles.current_position[id2] = particles.current_position[id2] - normalized_collision_vector * (1 - distance) / 2.0f;
							}
						}
					}
				}
				partitioner.partitions[i][j].ids.resize(0);
			}
		}

		if (thread_id < parallelism) { phase1_end_barrier.arrive_and_wait(); }
		else { phase2_end_barrier.arrive_and_wait(); }
	}

	void Update(float dt)
	{
		for (size_t i = 0; i < particles.GetNumberOfParticles(); i++) partitioner.MaybeInsertIntoPartition(i);

		phase1_start_barrier.arrive_and_wait();
		phase1_end_barrier.arrive_and_wait();
		phase2_start_barrier.arrive_and_wait();
		phase2_end_barrier.arrive_and_wait();
	}

	static void NoOp() noexcept {}

private:
	const Configuration& config;
	Particles& particles;
	Partitioner& partitioner;

	//Threading stuff.
	std::atomic<bool> running;
	std::vector<std::thread> phase1_threads;
	std::vector<std::thread> phase2_threads;
};
