// C library headers
#include <cstdio>  // printf
#include <cstring> // memset
#include <string>  // stoi

// Linux headers
#include <fcntl.h>	// file controls
#include <unistd.h> // read(), close()

int main()
{
	char read_buf[256];
	int enc_val[5];
	std::memset(&read_buf, '\0', sizeof(read_buf));
	const char device[] = "/dev/ttyACM0";
	int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	if (fd == -1)
	{
		printf("failed to open port\n");
	}
	// int n = read(fd, &read_buf, sizeof(read_buf));
	read(fd, &read_buf, sizeof(read_buf));
	char *p;
	p = std::strtok(read_buf, ",");
	for (int i = 0; i <= 5; i++)
	{
		std::printf("%c", read_buf[i]);
		p = std::strtok(NULL, ",");
		enc_val[i] = std::stoi(p);
	}
	close(fd);
	return 0;
}