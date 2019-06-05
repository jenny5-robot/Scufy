// Author: Mihai Oltean, mihaioltean.github.io, mihai.oltean@gmail.com
// Jenny 5 websites: jenny5.org, jenny5-robot.github.io/
// Jenny 5 source code: github.com/jenny5-robot
// MIT License
//--------------------------------------------------------------

#include "utils.h"

int8_t sign(int val) 
{
	if (val < 0) return -1;
	if (val == 0) return 0;
	return 1;
}