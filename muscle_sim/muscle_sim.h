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
	void calculate_final_muscle_position(std::array<double, 6> applied_pressure);
	std::array<int, 6> get_process_variable();
	std::array<int, 6> get_final_muscle_position();

	// I feel like these three methods should be private, but I also need to test them.
	// Best way to test these methods would be to refactor them in a private implementation class (Pimpl idiom)
	// and then test that class.
	// But since this class doesn't really need to be secure, I am forgoing security in favor of simplicity
	double up_poly_component(double x);
	double up_gaussian_component(double x);
	double down_poly_component(double x);

private: // begin private section
	// member variables
	const int min_distance = 0;			 // in millimeters
	const int max_distance = 200;		 //
	const int min_pressure = 0;			 // in millibars
	const int max_pressure = 6000;		 //
	const int muscle_response_time = 10; // milliseconds
	bool final_output_computed;

	std::array<int, 6> pressure_change_characteristic;
	// 0 = pressure was never applied before
	// 1 = pressure didn't change
	// 2 = drop in pressure from previous pressure values
	// 3 = increase in pressuure from previous pressure values

	std::array<double, 6> previously_applied_pressure;
	std::array<int, 6> current_muscle_position;
	std::array<int, 6> final_muscle_position;

	double current_time;
	double previous_time;
	std::array<double, 6> time_at_pressure_application;

	// private member function
};

#endif