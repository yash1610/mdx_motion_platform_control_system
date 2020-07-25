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
	pressure_change_characteristic = {0, 0, 0, 0, 0, 0};
	time_at_pressure_application = {-1000, -1000, -1000, -1000, -1000, -1000};
	final_output_computed = false;
}

// destructor
muscle_sim::~muscle_sim()
{
}

void muscle_sim::calculate_final_muscle_position(std::array<double, 6> applied_pressure)
{

	for (int i = 0; i < 6; i++)
	{
		if (applied_pressure[i] > previously_applied_pressure[i])
		{
			pressure_change_characteristic[i] = 3;
			time_at_pressure_application[i] = get_current_epoch_ms;
			final_muscle_position[i] = std::round(UP_POLY_COMPONENT(applied_pressure[i]) + UP_GAUSSIAN_COMPONENT(applied_pressure[i]));
		}
		else if (applied_pressure[i] < previously_applied_pressure[i])
		{
			pressure_change_characteristic[i] = 2;
			time_at_pressure_application[i] = get_current_epoch_ms;
			final_muscle_position[i] = std::round(DOWN_POLY_COMPONENT(applied_pressure[i]));
		}
		else if (applied_pressure[i] == 0 && final_output_computed == false)
		{
			pressure_change_characteristic[i] = 0;
			time_at_pressure_application[i] = get_current_epoch_ms;
			final_muscle_position[i] = 0;
		}
		else if (applied_pressure[i] == previously_applied_pressure[i] && final_output_computed == true)
		{
			// do nothing
		}
	}
	previously_applied_pressure = applied_pressure;
	std::printf("FM1 = %d, FM2 = %d, FM3 = %d, FM4 = %d, FM5 = %d, FM6 = %d\n", final_muscle_position[0], final_muscle_position[1], final_muscle_position[2], final_muscle_position[3], final_muscle_position[4], final_muscle_position[5]);
	std::printf("PCC1 = %d, PCC2 = %d, PCC3 = %d, PCC4 = %d, PCC5 = %d, PCC6 = %d\n", pressure_change_characteristic[0], pressure_change_characteristic[1], pressure_change_characteristic[2], pressure_change_characteristic[3], pressure_change_characteristic[4], pressure_change_characteristic[5]);
	final_output_computed = true;
}

std::array<int, 6> muscle_sim::get_process_variable()
{
	if (final_output_computed == false)
	{
		return current_muscle_position;
	}

	std::srand(std::time(nullptr)); // required for rng

	std::array<int, 6> process_variable;
	double time_diff;
	double tentative_position;

	for (int i = 0; i < 6; i++)
	{
		current_time = get_current_epoch_ms;

		time_diff = current_time - time_at_pressure_application[i];

		if (time_diff <= muscle_response_time || time_at_pressure_application[i] == -1000)
		// assuming that the muscles takes 100ms to respond to applied pressure, there won't be a change in muscle position untill 100ms have passed
		{
			process_variable[i] = current_muscle_position[i];
		}
		else
		{
			time_diff -= muscle_response_time;
			if (pressure_change_characteristic[i] == 3)
			{
				tentative_position = current_muscle_position[i] + (time_diff / 2.5); // assuming 2.5 ms taken for 1mm of change in movement
				if (tentative_position < final_muscle_position[i] - 1)
				{
					process_variable[i] = std::round(tentative_position) + (std::rand() % 5) - 2;
				}
				else
				{
					process_variable[i] = final_muscle_position[i];
				}
			}
			else if (pressure_change_characteristic[i] == 2)
			{
				tentative_position = current_muscle_position[i] - (time_diff / 2.5); // assuming 2.5 ms taken for 1mm of change in movement
				if (tentative_position > final_muscle_position[i] + 1)
				{
					process_variable[i] = std::round(tentative_position) + (std::rand() % 5) - 2;
				}
				else
				{
					process_variable[i] = final_muscle_position[i];
				}
			}
		}

		if (process_variable[i] < 0)
			process_variable[i] = 0;
		else if (process_variable[i] > 200)
			process_variable[i] = 200;
	}
	current_muscle_position = process_variable;
	std::printf("M1 = %d, M2 = %d, M3 = %d, M4 = %d, M5 = %d, M6 = %d\n", process_variable[0], process_variable[1], process_variable[2], process_variable[3], process_variable[4], process_variable[5]);
	return process_variable;
}

std::array<int, 6> muscle_sim::get_final_muscle_position()
{
	return final_muscle_position;
}