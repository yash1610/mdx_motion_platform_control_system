#include "encoder_io/encoder_io.h"
#include "muscle_sim/muscle_sim.h"
#include "pid/pid.h"
#include <fstream>
#include <ctime>
#include <iomanip>

#define get_current_epoch_ms std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

int main(int argc, char *argv[])
{
	int i = 0;
	int setpoint_frequency = 100;
	// timing variables
	double current_time;
	double previous_time = 0;
	double pid_loop_previous_time = 0;

	// initalize a runtime log file
	std::ofstream logfile;
	logfile.open("log.csv");
	logfile << "time,|,setpoint1,setpoint2,setpoint3,setpoint4,setpoint5,setpoint6,|,";
	logfile << "process_variable1,process_variable2,process_variable3,process_variable4,process_variable5,process_variable6,|,";
	logfile << "pressure1,pressure2,pressure3,pressure4,pressure5,pressure6\n";

	// initialize encoder
	encoder_io encoder;
	encoder.encoder_write('R');
	std::array<int, 6> setpoints;
	// initalize muscle_simulation
	muscle_sim muscle;
	std::array<int, 6> process_variable;

	// pid initialize
	double kp = std::atof(argv[1]);
	double ki = std::atof(argv[2]);
	double kd = std::atof(argv[3]);
	printf("kp = %f\nki = %f\nkd = %f\n", kp, ki, kd);
	double sample_rate = 5; //milliseconds
	double min_val = 0;		//millibars
	double max_val = 6000;	//millibars

	PID pid(sample_rate, kp, ki, kd, min_val, max_val);
	std::array<double, 6> pid_output;
	// end pid initialize

	std::srand(std::time(nullptr));
	double start = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	while (i == 0)
	{
		current_time = get_current_epoch_ms;
		if (current_time - pid_loop_previous_time >= sample_rate)
		{
			pid_loop_previous_time = current_time;

			// Get new setpoints every 50ms or 20hz
			current_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			std::time_t secsSinceEpoch = current_time;
			logfile << std::put_time(std::localtime(&secsSinceEpoch), "%H:%M:%S") << ",|,";

			std::printf("Current time - previous time = %f\n", current_time - previous_time);

			if (current_time - previous_time >= setpoint_frequency)
			{
				previous_time = current_time;
				// setpoints = encoder.encoder_io::encoder_values();
				// setpoints[0] = setpoints[2] + (std::rand() % 21) - 10;
				// setpoints[1] = setpoints[2] + (std::rand() % 21) - 10;
				// setpoints[3] = setpoints[2] + (std::rand() % 21) - 10;
				// setpoints[4] = setpoints[2] + (std::rand() % 21) - 10;
				// setpoints[5] = setpoints[2] + (std::rand() % 21) - 10;
				setpoints[0] = (std::rand() % 200);
				setpoints[1] = (std::rand() % 200);
				setpoints[2] = (std::rand() % 200);
				setpoints[3] = (std::rand() % 200);
				setpoints[4] = (std::rand() % 200);
				setpoints[5] = (std::rand() % 200);
				std::printf("new setpoints generated\n");
			}
			// end of getting setpoint

			logfile << setpoints[0] << "," << setpoints[1] << "," << setpoints[2] << "," << setpoints[3] << "," << setpoints[4] << "," << setpoints[5] << ",|,";

			process_variable = muscle.muscle_sim::get_process_variable();

			pid_output = pid.PID::compute(setpoints, process_variable);

			muscle.muscle_sim::calculate_final_muscle_position(pid_output);

			logfile << process_variable[0] << "," << process_variable[1] << "," << process_variable[2] << "," << process_variable[3] << "," << process_variable[4] << "," << process_variable[5] << ",|,";
			logfile << pid_output[0] << "," << pid_output[1] << "," << pid_output[2] << "," << pid_output[3] << "," << pid_output[4] << "," << pid_output[5] << "\n";

			double now = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
			double diff = now - start;
			if (diff >= 6)
			{
				i = 1;
			}
		}
	}
	return 1;
}