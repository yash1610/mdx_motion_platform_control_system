#include "../muscle_sim.h"
#include <gtest/gtest.h>
#include <array>
#include <chrono>
#include <thread>

TEST(muscle_function_test, upward_motion_test)
{
	muscle_sim muscle;

	EXPECT_EQ(std::round(muscle.muscle_sim::up_poly_component(100) + muscle.muscle_sim::up_gaussian_component(100)), 0);
	EXPECT_EQ(std::round(muscle.muscle_sim::up_poly_component(1975) + muscle.muscle_sim::up_gaussian_component(1975)), 50);
	EXPECT_EQ(std::round(muscle.muscle_sim::up_poly_component(2585) + muscle.muscle_sim::up_gaussian_component(2585)), 100);
	EXPECT_EQ(std::round(muscle.muscle_sim::up_poly_component(3645) + muscle.muscle_sim::up_gaussian_component(3645)), 150);
	EXPECT_EQ(std::round(muscle.muscle_sim::up_poly_component(6000) + muscle.muscle_sim::up_gaussian_component(6000)), 199);
	EXPECT_FALSE(std::round(muscle.muscle_sim::up_poly_component(6000) + muscle.muscle_sim::up_gaussian_component(6000)) == 0);
}

TEST(muscle_function_test, downward_motion_test)
{
	muscle_sim muscle;

	EXPECT_EQ(std::round(muscle.muscle_sim::down_poly_component(5500)), 199);
	EXPECT_EQ(std::round(muscle.muscle_sim::down_poly_component(4860)), 199);
	EXPECT_EQ(std::round(muscle.muscle_sim::down_poly_component(3250)), 150);
	EXPECT_EQ(std::round(muscle.muscle_sim::down_poly_component(2540)), 100);
	EXPECT_EQ(std::round(muscle.muscle_sim::down_poly_component(1900)), 50);
	EXPECT_EQ(std::round(muscle.muscle_sim::down_poly_component(20)), 0);
	EXPECT_FALSE(std::round(muscle.muscle_sim::down_poly_component(0)) == 199);
}

TEST(simulation_test, no_pressure_test)
{
	std::array<int, 6> output;
	muscle_sim muscle;
	std::array<double, 6> expected_output = {0, 0, 0, 0, 0, 0};
	std::array<double, 6> input_pressure = {0, 0, 0, 0, 0, 0};

	muscle.muscle_sim::calculate_final_muscle_position(input_pressure);
	output = muscle.muscle_sim::get_process_variable();

	for (int i = 0; i < 6; i++)
		EXPECT_TRUE(output[i] == 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(600));

	output = muscle.muscle_sim::get_process_variable();

	for (int i = 0; i < 6; i++)
		EXPECT_TRUE(output[i] == 0);
}

TEST(simulation_test, simple_pressure_test)
{
	bool test_result = false;
	std::array<int, 6> output;
	muscle_sim muscle;
	std::array<double, 6> input_pressure = {1000, 2000, 3000, 4000, 5000, 6000};

	muscle.muscle_sim::calculate_final_muscle_position(input_pressure);
	output = muscle.muscle_sim::get_process_variable();

	for (int i = 0; i < 6; i++)
		EXPECT_TRUE(output[i] == 0);

	std::this_thread::sleep_for(std::chrono::milliseconds(600));

	output = muscle.muscle_sim::get_process_variable();

	for (int i = 0; i < 6; i++)
	{
		if (i < 5 && output[i] > 0 && output[i] <= 200 && output[i] < output[i + 1])
		{
			test_result = true;
		}
		else if (i == 5 && output[i] > 0 && output[i] <= 200)
		{
			test_result = true;
		}
		EXPECT_TRUE(test_result);
	}
}

TEST(simulation_test, intermediate_muscle_positins)
{
	bool test_result = false;
	std::array<int, 6> output;
	std::array<int, 6> intermediate_output;
	muscle_sim muscle;
	std::array<double, 6> input_pressure = {1000, 2000, 3000, 4000, 5000, 6000};

	muscle.muscle_sim::calculate_final_muscle_position(input_pressure);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	intermediate_output = muscle.muscle_sim::get_process_variable();

	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	output = muscle.muscle_sim::get_process_variable();

	for (int i = 0; i < 6; i++)
	{
		EXPECT_LE(intermediate_output[i], output[i]);
	}
}

TEST(simulation_test, consecutive_presure_application)
{
	bool test_result = false;
	muscle_sim muscle;
	std::array<double, 6> input_pressure = {1000, 2000, 3000, 4000, 5000, 6000};
	std::array<int, 6> output;
	std::array<double, 6> input_pressure_2 = {500, 3000, 0, 6000, 5000, 100};
	std::array<int, 6> output_for_next_pressure;

	muscle.muscle_sim::calculate_final_muscle_position(input_pressure);
	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	output = muscle.muscle_sim::get_process_variable();

	muscle.muscle_sim::calculate_final_muscle_position(input_pressure_2);
	std::this_thread::sleep_for(std::chrono::milliseconds(600));
	output_for_next_pressure = muscle.muscle_sim::get_process_variable();

	for (int i = 0; i < 6; i++)
	{
		if (input_pressure[i] < input_pressure_2[i] && output[i] < output_for_next_pressure[i])
		{
			test_result = true;
		}
		else if (input_pressure[i] > input_pressure_2[i] && output[i] > output_for_next_pressure[i])
		{
			test_result = true;
		}
		else if (input_pressure[i] == input_pressure_2[i] && output[i] == output_for_next_pressure[i])
		{
			test_result = true;
		}
		EXPECT_TRUE(test_result);
	}
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}