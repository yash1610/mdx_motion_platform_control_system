#include <iostream>
#include "encoder_reader.h"

int main()
{
	while (true)
	{
		encoder_reader enc;
		printf("%d\n", enc.encoder_values(3));
		enc.~encoder_reader();
	}
}
