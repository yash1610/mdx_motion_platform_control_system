#include <iostream>
#include <cmath>
#include <fstream>
#define UP_POLY_COMPONENT(x) -0.0864580836854149382 - 0.0001004532392595508 * x - 0.0000023161937212715 * std::pow(x, 2) + 0.0000000132639645076 * std::pow(x, 3) - 0.0000000000036200247 * std::pow(x, 4) + 0.0000000000000002713 * std::pow(x, 5)
#define UP_GAUSSIAN_COMPONENT(x) 18.8062963858669363049 * std::exp(-std::log(2) * std::pow(((x - 2792.7889820722848526202) / 540.3774049635310348094), 2))

int main()
{
	std::ofstream myfile;
	myfile.open("example.txt");
	int x;
	double y;
	for (size_t i = 0; i < 6000; i++)
	{
		y = UP_POLY_COMPONENT(i) + UP_GAUSSIAN_COMPONENT(i);
		x = std::round(y);
		myfile << x << ", " << i << "\n";
	}
}
