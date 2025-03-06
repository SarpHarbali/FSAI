#include <stdio.h>
#include "physics.h"

char* stringifyInput(Input input){
	static char str[256];
    sprintf(str, "acc: %f | vel: %f | delta: %f\n", input.acc, input.vel, input.delta);
    return str;
}