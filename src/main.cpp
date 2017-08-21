
#include <iostream>
#include <chrono>
#include <thread>

#include "../include/IoPin.h"

using namespace std;

int main()
{
	IoPin NewPin(21, IoPin::Direction::Output);
	NewPin.On();
	this_thread::sleep_for(chrono::seconds(2));
	NewPin.Off();
	
	return 0;
}

