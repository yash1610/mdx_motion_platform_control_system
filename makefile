# Makefile for the MDX motion platform PID based controllers
# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o ./pid/pid.o ./muscle_sim/muscle_sim.o ./encoder_io/encoder_io.o
	$(CXX) $(CXXFLAGS) -o main main.o ./pid/pid.o ./muscle_sim/muscle_sim.o ./encoder_io/encoder_io.o

# The main.o target can be written more simply

main.o: main.cpp ./pid/pid.h ./muscle_sim/muscle_sim.h ./encoder_io/encoder_io.h
	$(CXX) $(CXXFLAGS) -c main.cpp

./pid/pid.o: ./pid/pid.h

./muscle_sim/muscle_sim.o: ./muscle_sim/muscle_sim.h

./encoder_io/encoder_io.o: ./encoder_io/encoder_io.h