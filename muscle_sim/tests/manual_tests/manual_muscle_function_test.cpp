#include <iostream>
#include <cmath>
#include <fstream>
#define UP_POLY_COMPONENT(x) -0.0864580836854149382 - 0.0001004532392595508 * x - 0.0000023161937212715 * std::pow(x, 2) + 0.0000000132639645076 * std::pow(x, 3) - 0.0000000000036200247 * std::pow(x, 4) + 0.0000000000000002713 * std::pow(x, 5)
#define UP_GAUSSIAN_COMPONENT(x) 18.8062963858669363049 * std::exp(-std::log(2) * std::pow(((x - 2792.7889820722848526202) / 540.3774049635310348094), 2))
#define DOWN_POLY_COMPONENT(x) -0.153534501375474974 + 0.016147973880630856 * x - 0.000044351093632348 * std::pow(x, 2) + 0.000000046557904348 * std::pow(x, 3) - 0.000000000012817817 * std::pow(x, 4) + 0.000000000000001097 * std::pow(x, 5);

void upward_motion_output()
{
	std::ofstream myfile;
	myfile.open("sim_fucntion_test.csv");
	int x;
	double y;
	myfile << "Distance, Pressure\n";
	for (int i = 0; i < 6000; i++)
	{
		y = UP_POLY_COMPONENT(i) + DOWN_POLY_COMPONENT(i);
		x = std::round(y);
		myfile << x << ", " << i << "\n";
	}
}

void downward_motion_output()
{
	std::ofstream myfile;
	myfile.open("sim_fucntion_test_2.csv");
	int x;
	double y;
	myfile << "Distance, Pressure\n";
	for (int i = 0; i < 6000; i++)
	{
		if (i <= 4874)
		{
			y = DOWN_POLY_COMPONENT(i);
		}
		else if (i >= 4875)
		{
			y = 199;
		}
		x = std::round(y);
		myfile << x << ", " << i << "\n";
	}
}

int main()
{
	upward_motion_output();
	downward_motion_output();
}

