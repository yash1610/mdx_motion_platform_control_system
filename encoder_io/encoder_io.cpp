#include "encoder_io.h"
#define BAUDRATE B57600

encoder_io::encoder_io()
{
	fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
	fcntl(fd, F_SETFL, 0);
	if (fd == -1)
	{
		printf("failed to open port\n");
	}
}

encoder_io::~encoder_io()
{
	close(fd);
}

void encoder_io::read_encoder_values()
{
	char read_buf[255];
	char *p;
	std::memset(&read_buf, '\0', sizeof(read_buf));
	int n = read(fd, read_buf, sizeof(read_buf));
	if (n > 0)
	{
		read_buf[n] = 0;
		p = std::strtok(read_buf, ",");
		for (int i = 0; i <= 5; i++)
		{
			p = std::strtok(NULL, ",");
			enc_val[i] = std::stoi(p);
		}
	}
	else if (n < 0)
	{
		printf("Error from read: %d: %s\n", n, strerror(errno));
	}
	else
	{
		printf("Nothing read. EOF?\n");
	}
}

std::array<int, 6> encoder_io::encoder_values()
{
	encoder_io::read_encoder_values();
	return enc_val;
}

int encoder_io::encoder_values(int x)
{
	encoder_io::read_encoder_values();
	return enc_val[x];
}

void encoder_io::encoder_write(char x)
{
	write(fd, &x, 1);
}