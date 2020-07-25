#include "../muscle_sim.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>
#include <iomanip>
#define get_current_epoch_ms std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

// Test with changing input pressure before the simulated muscle has reached final position for the previously applied muscle

int main()
{
	int i = 0;
	int setpoint_frequency = 50;
	//timing variables
	double current_time;
	double previous_time = 0;
	double loop_previous_time = 0;
	double sample_rate = 10;

	//initalize a runtime log file
	std::ofstream logfile;
	logfile.open("sim_test.csv");
	logfile << "";
	std::array<int, 6> process_variable;
	std::array<double, 6> applied_pressure;
	std::array<int, 6> output;
	std::array<int, 6> final_output;
	//initialize encoder
	muscle_sim muscle;

	std::array<double, 6> input_pressure = {1000, 2000, 3000, 4000, 5000, 6000};

	muscle.muscle_sim::calculate_final_muscle_position(input_pressure);
	final_output = muscle.muscle_sim::get_final_muscle_position();
	std::printf("FM1 = %d, FM2 = %d, FM3 = %d, FM4 = %d, FM5 = %d, FM6 = %d\n", final_output[0], final_output[1], final_output[2], final_output[3], final_output[4], final_output[5]);

	double start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::printf("A = %f\n", start);
	std::this_thread::sleep_for(std::chrono::milliseconds(250));

	start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::printf("B after 250ms = %f\n", start);
	output = muscle.muscle_sim::get_process_variable();
	std::printf("M1 = %d, M2 = %d, M3 = %d, M4 = %d, M5 = %d, M6 = %d\n", output[0], output[1], output[2], output[3], output[4], output[5]);

	input_pressure = {6000, 1000, 0, 2000, 1000, 3000};
	muscle.muscle_sim::calculate_final_muscle_position(input_pressure);
	final_output = muscle.muscle_sim::get_final_muscle_position();
	std::printf("FM1 = %d, FM2 = %d, FM3 = %d, FM4 = %d, FM5 = %d, FM6 = %d\n", final_output[0], final_output[1], final_output[2], final_output[3], final_output[4], final_output[5]);

	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::printf("CC after 50ms = %f\n", start);
	output = muscle.muscle_sim::get_process_variable();
	std::printf("M1 = %d, M2 = %d, M3 = %d, M4 = %d, M5 = %d, M6 = %d\n", output[0], output[1], output[2], output[3], output[4], output[5]);

	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	std::printf("CC after 300ms = %f\n", start);
	output = muscle.muscle_sim::get_process_variable();
	std::printf("M1 = %d, M2 = %d, M3 = %d, M4 = %d, M5 = %d, M6 = %d\n", output[0], output[1], output[2], output[3], output[4], output[5]);
}