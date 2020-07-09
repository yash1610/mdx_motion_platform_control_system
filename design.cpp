int array_of_encoder_values;
int array_of_setpoint;
int array_of_desired_actuator_positions;

void calibration()
{
	//reset encoders and actuators to min and max positions also set the max_values of encoders / actuators
}

int receive_encoder_values()
{
	//receive encoder values and save the current position in an array of length 6. Similar to what it already does

	return array_of_encoder_values;
}

int receive_messages_from_host()
{
	// this function be responsible for reading messages published by the vr simulation pc or any host.
	// Processing and saving the message in an array of length 6 for usage

	return array_of_setpoint;
}

void commands_to_festo(int array_of_setpoint)
{
	// provides the program output to the FESTO
}

void pid(int *array_of_encoder_values, int *array_of_desired_actuator_positions)
{

	//Tuning parameters
	//Set the actuators to new positions using the error rate and the tuning parameters

	return commands_to_festo(array_of_setpoint);
}

void pid(int enocoder_number, int desired_actuator_position)
{
}

int main()
{

	// 2 threads running synchronously

	// 	1st thread
	receive_messages_from_host();									   //at 20hz 2nd thread
	pid(array_of_encoder_values, array_of_desired_actuator_positions); //running at a yet to be determined frequency

	//This can also be done without using threads.If it is to be run on a microcontroller.
	// 	Where both functions are executed linearly,
	// 	making the overall frequency 20Hz
	// 	20Hz because that's the freuquency of output from the host pc
}