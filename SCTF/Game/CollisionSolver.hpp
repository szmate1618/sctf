#pragma once

#include <thread>
#include <shared_mutex>
#include <atomic>
#include <chrono>
#include <iostream>
#include <barrier>
#include <functional>

#include "Particles.hpp"
#include "Partitioner.hpp"

using namespace std::chrono_literals;


std::barrier phase1_barrier(5);
std::barrier phase2_barrier(5);

class CollisionSolver
{
public:
	CollisionSolver(const Configuration& config, Partitioner& partitioner, Particles& particles):
		config{ config },
		particles{ particles },
		partitioner{ partitioner },
		running{ true }
	{
		std::vector<size_t> column_indices[8];
		size_t columns_per_thread = partitioner.number_of_columns / 4;
		for (size_t i = 0; i < columns_per_thread - 1; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				column_indices[j].push_back(j * columns_per_thread + i);
			}
		}
		for (size_t j = 0; j < 4; j++)
		{
			column_indices[4 + j].push_back((j + 1) * columns_per_thread - 1);
		}

		worker_threads.emplace_back([this, column_indices]() { while (running) { UpdateColumns(column_indices[0], 0); } });
		worker_threads.emplace_back([this, column_indices]() { while (running) { UpdateColumns(column_indices[1], 1); } });
		worker_threads.emplace_back([this, column_indices]() { while (running) { UpdateColumns(column_indices[2], 2); } });
		worker_threads.emplace_back([this, column_indices]() { while (running) { UpdateColumns(column_indices[3], 3); } });
		worker_threads.emplace_back([this, column_indices]() { while (running) { UpdateColumns2(column_indices[4], 4); } });
		worker_threads.emplace_back([this, column_indices]() { while (running) { UpdateColumns2(column_indices[5], 5); } });
		worker_threads.emplace_back([this, column_indices]() { while (running) { UpdateColumns2(column_indices[6], 6); } });
		worker_threads.emplace_back([this, column_indices]() { while (running) { UpdateColumns2(column_indices[7], 7); } });
	}

	~CollisionSolver()
	{
		running = false;
		phase1_barrier.arrive_and_wait();
		phase2_barrier.arrive_and_wait();
		for (size_t i = 0; i < num_threads; i++)
		{
			worker_threads[i].join();
		}
		for (size_t i = 0; i < num_threads; i++)
		{
			worker_threads[4 + i].join();
		}
	}

	void UpdateColumns(const std::vector<size_t>& column_indices, size_t thread_id)
	{
		phase1_barrier.arrive_and_wait();
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

		phase1_barrier.arrive_and_wait();
	}

	//TODO: Code duplication.
	void UpdateColumns2(const std::vector<size_t>& column_indices, size_t thread_id)
	{
		phase2_barrier.arrive_and_wait();
		if (!running) return;

		for (size_t i : column_indices)
		{
			for (size_t j = 0; j < partitioner.number_of_rows; j++)
			{
				if (partitioner.partitions[i][j].ids.size() >= 2)
				{
					for (size_t k = 0; k < partitioner.partitions[i][j].ids.size() - 1; k++)
					{
						for (size_t l = k + 1; l < partitioner.partitions[i][j].ids.size(); l++)
						{
							const auto id1 = partitioner.partitions[i][j].ids[k];
							const auto id2 = partitioner.partitions[i][j].ids[l];
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

		phase2_barrier.arrive_and_wait();
	}

	void Update(float dt)
	{
		for (size_t i = 0; i < particles.GetNumberOfParticles(); i++) partitioner.MaybeInsertIntoPartition(i);

		phase1_barrier.arrive_and_wait();
		phase1_barrier.arrive_and_wait();
		phase2_barrier.arrive_and_wait();
		phase2_barrier.arrive_and_wait();
	}

	static void NoOp() noexcept {}

private:
	const Configuration& config;
	Particles& particles;
	Partitioner& partitioner;

	//Threading stuff.
	bool running;
	const int num_threads = 4;
	std::vector<std::thread> worker_threads;
	std::atomic<size_t> threads_ready = 0;
	std::atomic<size_t> threads_done = 0;
};
