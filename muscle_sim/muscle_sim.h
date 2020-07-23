#ifndef MUSCLE_SIM_H // include guard
#define MUSCLE_SIM_H
// C++ library headers
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <array>
#include <cmath>
#include <chrono>

// learn about RAII and Rule of three

class muscle_sim // declaration of the class
{
public:			   // begin public section
	muscle_sim();  // constructor
	~muscle_sim(); // destructor
	std::array<int, 6> calculate_final_muscle_position(std::array<double, 6> applied_pressure);
	std::array<int, 6> get_process_variable();

private: // begin private section
	// member variables
	const int min_distance = 0;	   // in millimeters
	const int max_distance = 200;  //
	const int min_pressure = 0;	   // in millibars
	const int max_pressure = 6000; //

	std::array<double, 6> previously_applied_pressure;
	std::array<int, 6> current_muscle_position;
	std::array<int, 6> final_muscle_position;

	double current_time;
	double previous_time;
	double time_at_pressure_application;

	// private member function
};

#endif