#include <stdio.h>
#include "physics.h"

char printString(Input input){
	printf("acc: %f | vel: %f | delta: %f\n", input.acc, input.vel, input.delta);
}