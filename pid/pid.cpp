#include "pid.h"

// constructor
PID::PID(double sample_rate, double kp, double ki, double kd, double min_val, double max_val)
{
	i_sample_rate = sample_rate;
	i_kp = kp;
	i_kd = ki;
	i_ki = kd;
	i_min_val = min_val;
	i_max_val = max_val;
	pre_error = {0, 0, 0, 0, 0, 0};
	accumulated_integral_error = {0, 0, 0, 0, 0, 0};
	last_process_variable = {0, 0, 0, 0, 0, 0};
	accumulated_integral_out = {0, 0, 0, 0, 0, 0};
}

PID::~PID()
{
}

// function to change kp, ki, kd parameters
void PID::change_tuning_params(double kp, double ki, double kd)
{
	i_kp = kp;
	i_kd = ki;
	i_ki = kd;
}

// function to change sample rate
void PID::change_sample_rate(double sample_rate)
{
	i_sample_rate = sample_rate;
}

// function to change maximum and minimum values of the system/plant
void PID::change_plant_min_max_values(double min_val, double max_val)
{
	i_min_val = min_val;
	i_max_val = max_val;
}

//function to get sample_rate of the PID loop
double PID::get_sample_rate() const
{
	return i_sample_rate;
}

// PID with integral and output clamping and also additional support for
// mid operation tuning parameter changes and derivative on measurement.
std::array<double, 6> PID::compute(std::array<int, 6> setpoint, std::array<int, 6> process_variable)
{
	std::array<double, 6> error;
	std::array<double, 6> p_out;
	std::array<double, 6> derivative;
	std::array<double, 6> d_out;
	std::array<double, 6> output;

	for (int i = 0; i < 6; i++)
	{
		// Calculate error
		error[i] = setpoint[i] - process_variable[i];

		// Proportional term
		p_out[i] = i_kp * error[i];

		// Integral term with support for mid-operation change in tuning parameter
		accumulated_integral_out[i] += i_ki * error[i] * i_sample_rate;

		// integral clamping
		if (accumulated_integral_out[i] > i_max_val)
			accumulated_integral_out[i] = i_max_val;
		else if (accumulated_integral_out[i] < i_min_val)
			accumulated_integral_out[i] = i_min_val;

		// Derivative term with derivative on measurement
		derivative[i] = (process_variable[i] - last_process_variable[i]) / i_sample_rate;
		d_out[i] = i_kd * derivative[i];

		// Calculate total output
		output[i] = p_out[i] + accumulated_integral_out[i] - d_out[i];

		// Output clamping
		if (output[i] > i_max_val)
			output[i] = i_max_val;
		else if (output[i] < i_min_val)
			output[i] = i_min_val;

		// Save process_variable to  to last_process_variable
		last_process_variable[i] = process_variable[i];
	}
	return output;
}

// Standard PID with integral and output clamping
std::array<double, 6> PID::simple_compute(std::array<int, 6> setpoint, std::array<int, 6> process_variable)
{
	std::array<double, 6> error;
	std::array<double, 6> p_out;
	std::array<double, 6> i_out;
	std::array<double, 6> derivative;
	std::array<double, 6> d_out;
	std::array<double, 6> output;

	for (int i = 0; i < 6; i++)
	{
		// Calculate error
		error[i] = setpoint[i] - process_variable[i];

		// Proportional term
		p_out[i] = i_kp * error[i];

		// Integral term
		accumulated_integral_error[i] += (error[i] * i_sample_rate);
		i_out[i] = i_ki * accumulated_integral_error[i];

		// integral clamping
		if (i_out[i] > i_max_val)
			i_out[i] = i_max_val;
		else if (i_out[i] < i_min_val)
			i_out[i] = i_min_val;

		// Derivative term
		derivative[i] = (error[i] - pre_error[i]) / i_sample_rate;
		d_out[i] = i_kd * derivative[i];

		// Calculate total output
		output[i] = p_out[i] + i_out[i] + d_out[i];

		// Output clamping
		if (output[i] > i_max_val)
			output[i] = i_max_val;
		else if (output[i] < i_min_val)
			output[i] = i_min_val;

		// Save error to previous error
		pre_error[i] = error[i];
	}
	return output;
}