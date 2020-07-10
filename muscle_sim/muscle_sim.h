#ifndef MUSCLE_SIM_H // include guard
#define MUSCLE_SIM_H
// C++ library headers
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <array>
#include <cmath>

// learn about RAII and Rule of three

class muscle_sim // declaration of the class
{
public:			   // begin public section
	muscle_sim();  // constructor
	~muscle_sim(); // destructor
	std::array<int, 6> calculate_process_variable(std::array<double, 6> applied_pressure);

private: // begin private section
	// member variables
	const int min_distance = 0;	   // in millimeters
	const int max_distance = 200;  //
	const int min_pressure = 0;	   // in millibars
	const int max_pressure = 6000; //
	const int max_latency = 50;	   // in milliseconds, tentative value
	const int min_latency = 10;	   //

	std::array<int, 6> pressure;
	std::array<double, 6> previous_pressure;
	std::array<double, 6> current_muscle_position;
	int muscle_response_latency;

	// private member function
	int rand_latency(); // generates a random latency
};

#endif