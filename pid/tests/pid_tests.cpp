#include "../pid.h"
#include <gtest/gtest.h>

TEST(pid_values_test, sample_rate_test)
{
	double sample_rate = 10;
	double kp = 100;
	double ki = 10;
	double kd = 10;
	double min_val = 0;
	double max_val = 200;
	PID test_pid(sample_rate, kp, ki, kd, min_val, max_val);
	ASSERT_EQ(test_pid.PID::get_sample_rate(), 10);
	test_pid.PID::change_sample_rate(10.0001);
	ASSERT_EQ(test_pid.PID::get_sample_rate(), 10.0001);
	ASSERT_NE(test_pid.PID::get_sample_rate(), 10);
}

TEST(pid_values_test, tuning_parameters_test)
{
	std::array<double, 3> tuning_params_output;
	double sample_rate = 10;
	double kp = 5.000123;
	double ki = 105.15112;
	double kd = 2112332;
	double min_val = 0;
	double max_val = 200;
	PID test_pid(sample_rate, kp, ki, kd, min_val, max_val);

	tuning_params_output = test_pid.PID::get_tuning_params();
	ASSERT_EQ(tuning_params_output[0], 5.000123);
	ASSERT_EQ(tuning_params_output[1], 105.15112);
	ASSERT_EQ(tuning_params_output[2], 2112332);

	test_pid.PID::change_tuning_params(10, 115, 0.00001);

	tuning_params_output = test_pid.PID::get_tuning_params();
	ASSERT_EQ(tuning_params_output[0], 10);
	ASSERT_EQ(tuning_params_output[1], 115);
	ASSERT_EQ(tuning_params_output[2], 0.00001);

	ASSERT_NE(tuning_params_output[0], 5.000123);
	ASSERT_NE(tuning_params_output[1], 105.15112);
	ASSERT_NE(tuning_params_output[2], 2112332);
}

TEST(pid_values_test, min_max_value_test)
{
	std::array<double, 2> min_max_output;
	double sample_rate = 10;
	double kp = 10;
	double ki = 10;
	double kd = 10;
	double min_val = 0;
	double max_val = 200;
	PID test_pid(sample_rate, kp, ki, kd, min_val, max_val);

	min_max_output = test_pid.PID::get_min_max_values();
	ASSERT_EQ(min_max_output[0], 0);
	ASSERT_EQ(min_max_output[1], 200);

	test_pid.PID::change_plant_min_max_values(400, 1000);

	min_max_output = test_pid.PID::get_min_max_values();
	ASSERT_EQ(min_max_output[0], 400);
	ASSERT_EQ(min_max_output[1], 1000);

	ASSERT_NE(min_max_output[0], 0);
	ASSERT_NE(min_max_output[1], 200);
}

TEST(pid_compute_test, compute_test)
{
	std::array<double, 6> compute_output;
	std::array<double, 6> previous_compute_output;
	double sample_rate = 10;
	double kp = 0.3;
	double ki = 0.15;
	double kd = 0.1;
	double min_val = 0;
	double max_val = 6000;
	std::array<int, 6> setpoint = {100, 200, 300, 400, 500, 600};
	std::array<int, 6> process_variable = {50, 100, 150, 200, 250, 300};

	PID test_pid(sample_rate, kp, ki, kd, min_val, max_val);

	for (int i = 0; i < 25; i++)
	{
		compute_output = test_pid.PID::compute(setpoint, process_variable);
		for (int i = 0; i < 6; i++)
		{
			EXPECT_TRUE(0 >= compute_output[i] <= 6000 && compute_output[i] >= previous_compute_output[i]);
		}
		previous_compute_output = compute_output;
	}
}

TEST(pid_compute_test, simple_compute_test)
{
	std::array<double, 6> compute_output;
	std::array<double, 6> previous_compute_output;
	double sample_rate = 10;
	double kp = 0.3;
	double ki = 0.15;
	double kd = 0.1;
	double min_val = 0;
	double max_val = 6000;
	std::array<int, 6> setpoint = {100, 200, 300, 400, 500, 600};
	std::array<int, 6> process_variable = {50, 100, 150, 200, 250, 300};

	PID test_pid(sample_rate, kp, ki, kd, min_val, max_val);

	for (int i = 0; i < 25; i++)
	{
		compute_output = test_pid.PID::simple_compute(setpoint, process_variable);
		for (int i = 0; i < 6; i++)
		{
			EXPECT_TRUE(0 >= compute_output[i] <= 6000 && compute_output[i] >= previous_compute_output[i]);
		}
		previous_compute_output = compute_output;
	}
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}