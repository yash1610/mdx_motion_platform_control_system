C++ library that implements a MIMO-PID controller for a Pneumatic based Non-Linear Stewart Platform

Due to the non-linear design of the MDX VR Platform, designing an accurate control system was required. PID controllers have existed for over a 100 years, as such are very well tested; and owing to their simple closed-loop design, are also quite robust in nature. This project deals with the creation of such a controller. Albeit one that is scalable, and standardised using a Multi-Input-Multi-Output PID Controller that can be run on x86 or ARM hardware with any OS, or even with a simple microcontroller.


Requirements

make
googletest 1.10
