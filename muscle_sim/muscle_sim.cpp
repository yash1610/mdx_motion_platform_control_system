#include "muscle_sim.h"
#define UP_POLY_COMPONENT(x) -0.0864580836854149382 - 0.0001004532392595508 * x - 0.0000023161937212715 * std::pow(x, 2) + 0.0000000132639645076 * std::pow(x, 3) - 0.0000000000036200247 * std::pow(x, 4) + 0.0000000000000002713 * std::pow(x, 5)
#define UP_GAUSSIAN_COMPONENT(x) 18.8062963858669363049 * std::exp(-std::log(2) * std::pow(((x - 2792.7889820722848526202) / 540.3774049635310348094), 2))
#define DOWN_POLY_COMPONENT(x) -0.153534501375474974 + 0.016147973880630856 * x - 0.000044351093632348 * std::pow(x, 2) + 0.000000046557904348 * std::pow(x, 3) - 0.000000000012817817 * std::pow(x, 4) + 0.000000000000001097 * std::pow(x, 5)
#define get_current_epoch_ms std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

// constructor of muscle_sim,
muscle_sim::muscle_sim()
{
	current_muscle_position = {0, 0, 0, 0, 0, 0};
	final_muscle_position = {0, 0, 0, 0, 0, 0};
	previously_applied_pressure = {0, 0, 0, 0, 0, 0};
}

// destructor
muscle_sim::~muscle_sim()
{
}

std::array<int, 6> muscle_sim::calculate_final_muscle_position(std::array<double, 6> applied_pressure)
{
	for (int i = 0; i < 6; i++)
	{
		if (applied_pressure[i] > previously_applied_pressure[i])
		{
			final_muscle_position[i] = std::round(UP_POLY_COMPONENT(applied_pressure[i]) + UP_GAUSSIAN_COMPONENT(applied_pressure[i]));
		}
		else if (applied_pressure[i] < previously_applied_pressure[i])
		{
			final_muscle_position[i] = std::round(DOWN_POLY_COMPONENT(applied_pressure[i]));
		}
		else
		{
			final_muscle_position[i] = current_muscle_position[i];
		}
	}
	previously_applied_pressure = applied_pressure;
	time_at_pressure_application = get_current_epoch_ms;
}

std::array<int, 6> muscle_sim::get_process_variable()
{
	std::srand(std::time(nullptr));
	std::array<int, 6> process_variable;
	current_time = get_current_epoch_ms;
	double time_diff = current_time - time_at_pressure_application;
	if (time_diff <= 100)
	// assuming that the muscles takes 100ms to respond to applied pressure, there won't be a change in muscle position untill 100ms have passed
	{
		process_variable = current_muscle_position;
		return process_variable;
	}
	else
	{
		time_diff -= 100;
		for (int i = 0; i < 6; i++)
		{
			double tentative_position = current_muscle_position[i] + (time_diff / 2.5); // assuming 2.5 ms taken for 1mm of change in movement
			if (tentative_position < final_muscle_position[i] - 1)
			{
				process_variable[i] = std::round(tentative_position) + (std::rand() % 5) - 2;
			}
			else
			{
				process_variable[i] = final_muscle_position[i];
			}
		}
		current_muscle_position = process_variable;
		return process_variable;
	}
}