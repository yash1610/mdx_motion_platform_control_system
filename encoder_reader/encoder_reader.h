#ifndef ENCODER_READER_H // include guard
#define ENCODER_READER_H
// C library headers
#include <cstdio>  // printf
#include <cstring> // memset
#include <string>  // stoi
#include <array>   // std::aray

// Linux headers
#include <fcntl.h>	// file controls
#include <unistd.h> // read(), close()
#include <errno.h>
#include <termios.h> // Contains POSIX terminal control definitions

class encoder_reader // declaration of the class
{
public:									 // begin public section
	encoder_reader();					 // constructor
	~encoder_reader();					 // destructor
	std::array<int, 6> encoder_values(); // accessor function
	int encoder_values(int x);			 // accessor function for a single encoder

private: // begin private section
	// member variables
	std::array<int, 6> enc_val;
	int fd;
	//private functions
	void read_encoder_values();
};

#endif