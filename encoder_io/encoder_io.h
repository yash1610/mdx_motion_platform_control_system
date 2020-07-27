#ifndef ENCODER_IO_H // include guard
#define ENCODER_IO_H
// C library headers
#include <cstdio>  // printf
#include <cstring> // memset
#include <string>  // stoi
#include <array>   // std::aray
#include <stdexcept>

// Linux headers
#include <fcntl.h>	// file controls
#include <unistd.h> // read(), close()
#include <errno.h>
#include <termios.h> // Contains POSIX terminal control definitions

class encoder_io // declaration of the class
{
public:									 // begin public section
	encoder_io(const char *port);		 // constructor
	~encoder_io();						 // destructor
	std::array<int, 6> encoder_values(); // accessor function
	int encoder_values(int x);			 // accessor function for a single encoder
	void encoder_write(char x);

private: // begin private section
	// member variables
	std::array<int, 6> enc_val;
	int fd;
	//private functions
	void read_encoder_values();
};

#endif