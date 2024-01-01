#pragma once

struct PrecalculatedRandomNumbers {
	float random_numbers[100];
	size_t current_index;

	PrecalculatedRandomNumbers() : current_index{ 4 }
	{
		random_numbers[0] = 0.1;
		random_numbers[0] = -0.1;
		random_numbers[0] = 0.01;
		random_numbers[0] = -0.9;
	}

	float GetNext()
	{
		current_index = (current_index + 1) % 4;
		return random_numbers[current_index];
	}
};
