#ifndef PID_H // include guard
#define PID_H
// C++ library headers
#include <iostream>
#include <cmath>
#include <array>

class PID
{
public:
	// constructor
	PID(double sample_rate, double kp, double ki, double kd, double min_val, double max_val);

	// deconstructor
	~PID();

	// PID output compute functions

	// PID with integral and output clamping and also additional support for
	// mid operation tuning parameter changes and derivative on measurement.
	std::array<double, 6> compute(std::array<int, 6> setpoint, std::array<int, 6> process_variable);

	// Standard PID with integral and output clamping
	std::array<double, 6> simple_compute(std::array<int, 6> setpoint, std::array<int, 6> process_variable);

	// editing functions

	// function to change kp, ki, kd parameters
	void change_tuning_params(double kp, double ki, double kd);
	// function to change sample rate
	void change_sample_rate(double sample_rate);
	// function to change maximum and minimum values of the system/plant
	void change_plant_min_max_values(double min_val, double max_val);

	// getter functions

	//function to get sample_rate of the PID loop
	double get_sample_rate() const;

private:
	double i_sample_rate; // sample rate for the PID
	double i_max_val;	  // max - maximum value of manipulated variable
	double i_min_val;	  // min - minimum value of manipulated variable
	double i_kp;		  // Kp -  proportional gain
	double i_ki;		  // Ki -  Integral gain
	double i_kd;		  // Kd -  derivative gain
	std::array<double, 6> pre_error;
	std::array<double, 6> last_process_variable;
	std::array<double, 6> accumulated_integral_error;
	std::array<double, 6> accumulated_integral_out;
};

#endif