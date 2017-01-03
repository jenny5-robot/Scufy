
#include "utils.h"

int8_t sign(int val) 
{
	if (val < 0) return -1;
	if (val == 0) return 0;
	return 1;
}