#include "muscle_sim.h"
#define UP_POLY_COMPONENT(x) -0.0864580836854149382 - 0.0001004532392595508 * x - 0.0000023161937212715 * std::pow(x, 2) + 0.0000000132639645076 * std::pow(x, 3) - 0.0000000000036200247 * std::pow(x, 4) + 0.0000000000000002713 * std::pow(x, 5)
#define UP_GAUSSIAN_COMPONENT(x) 18.8062963858669363049 * std::exp(-std::log(2) * std::pow(((x - 2792.7889820722848526202) / 540.3774049635310348094), 2))
#define DOWN_POLY_COMPONENT(x) -0.153534501375474974 + 0.016147973880630856 * x - 0.000044351093632348 * std::pow(x, 2) + 0.000000046557904348 * std::pow(x, 3) - 0.000000000012817817 * std::pow(x, 4) + 0.000000000000001097 * std::pow(x, 5)
// constructor of muscle_sim,
muscle_sim::muscle_sim()
{
	current_muscle_position = {0, 0, 0, 0, 0, 0};
	previous_pressure = {0, 0, 0, 0, 0, 0};
}

// destructor
muscle_sim::~muscle_sim()
{
	delete &muscle_response_latency;
	delete &max_latency;
	delete &min_latency;
	delete &min_distance;
	delete &max_distance;
	delete &min_pressure;
	delete &max_pressure;
}

std::array<int, 6> muscle_sim::calculate_process_variable_using_function(std::array<double, 6> applied_pressure)
{
	std::array<int, 6> process_variable;
	for (int i = 0; i < 6; i++)
	{
		if (applied_pressure[i] > previous_pressure[i])
		{
			current_muscle_position[i] = UP_POLY_COMPONENT(applied_pressure[i]) + UP_GAUSSIAN_COMPONENT(applied_pressure[i]);
			process_variable[i] = std::round(current_muscle_position[i]);
		}
		else if (applied_pressure[i] < previous_pressure[i])
		{
			current_muscle_position[i] = DOWN_POLY_COMPONENT(applied_pressure[i]);
			process_variable[i] = std::round(current_muscle_position[i]);
		}
		else
		{
			process_variable[i] = std::round(current_muscle_position[i]);
		}
	}
	previous_pressure = applied_pressure;
	return process_variable;
}

// generates and returns a random latency
int muscle_sim::rand_latency()
{
	std::srand(std::time(nullptr));
	muscle_response_latency = (std::rand() % max_latency) + min_latency;
	return muscle_response_latency;
}
