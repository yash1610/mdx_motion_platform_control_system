#include <iostream>
#include "encoder_io.h"

int main()
{
	int i = 0;
	std::array<int, 6> x;
	encoder_io enc;
	enc.encoder_write('R');
	while (true)
	{
		x = enc.encoder_values();
		printf("%d, %d, %d, %d, %d, %d \n", x[0], x[1], x[2], x[3], x[4], x[5]);
		i++;
	}
	while (i <= 200)
	{
		x[0] = enc.encoder_values(3);
		printf("%d ---", x[0]);
		i++;
	}
	enc.~encoder_io();
	// encoder_io enc;
	// while (i <= 10)
	// {
	// 	printf("%d\n", enc.encoder_values(3));
	// 	i++;
	// }
	// enc.encoder_write('R');
	// while (i <= 20)
	// {
	// 	printf("%d\n", enc.encoder_values(3));
	// 	i++;
	// }
}
