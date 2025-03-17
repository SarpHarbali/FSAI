#include <math.h>
#include <stdio.h>
#include "controller.h"

// #define MAX(x, y) (((x) > (y)) ? (x) : (y))
// #define MIN(x, y) (((x) < (y)) ? (x) : (y))

double max(double a, double b){
	return (a > b) ? a : b;
}

double min(double a, double b){
	return (a<b) ? a : b;
}


double signedAngle2(Vector2 a, Vector2 b) {
    double dot = a.x * b.x + a.y * b.y;         
    double det = a.x * b.y - a.y * b.x;         
    double angle = atan2(det, dot);            
    return angle * (180.0f / M_PI); //M_PI is pi          
}

//CONSTANTS

double speedLookAheadSensitivity = 0.7;
Vector2 checkpointPositions[] = {
	{15.92, 39.42},
};
int numCheckpoints = 1;
double v = 12.25961;
Vector2 pos = {19.34, 24.99};
Vector2 forward = {-0.83, 0.56};
double epsilon = 0.000001;
double accelerationFactor = 0.002;



int approx(double a, double b){
	if (a-b<epsilon | b-a<epsilon){
		return 1;
	}
	return 0;
}

double Magnitude2 (Vector2 vector2){
	double x = vector2.x;
	double y = vector2.y;
	return sqrt((x*x)+(y*y)); 
}


double calculateExpectedSpeed(double angle){
	if (approx(angle, 0))
	{
		return 30.1964649875f;
	}
	double expression = fabs(1 / (accelerationFactor * angle));
	return MIN(expression, 30.1964649875f);
}

double clamp(double d, double min, double max){
	const double t = d < min ? min : d;
	return t > max ? max : t;
}

double computeThrottle(double v, Vector2 *checkpointPositions, int numCheckpoints, Vector2 pos, Vector2 forward){
	int lookaheadIndex = MIN(speedLookAheadSensitivity*v + 1, numCheckpoints);
	// Vector2 directionToCheckpoint.x = checkpointPositions[lookaheadIndex].x - pos.x;
	// Vector2 directionToCheckpoint.y = checkpointPositions[lookaheadIndex].y - pos.y;
	
	double x = checkpointPositions[lookaheadIndex].x - pos.x;
	double y = checkpointPositions[lookaheadIndex].y - pos.y;
	Vector2 directionToCheckpoint = { .x = fabs(x), .y = fabs(y) };
	double angleToCheckpoint = signedAngle2(forward, directionToCheckpoint);
	printf("Direction x is %f | Direction y is %f | Angle to checkpoint is %f\n", directionToCheckpoint.x, directionToCheckpoint.y, angleToCheckpoint);
	

	double expectedSpeed = calculateExpectedSpeed(angleToCheckpoint);
	printf("expectedSpeed is: %f\n", expectedSpeed);

	double distanceToCheckpoint = Magnitude2(directionToCheckpoint);

	double accelerationNeeded = v*(expectedSpeed - v) / distanceToCheckpoint;

	printf("accelerationNeeded is: %f\n", accelerationNeeded);

	double positiveThrottle = MAX(0, (1.0/3.4323432343)*accelerationNeeded);
	// double negativeThrottle = MAX(0, (1/10)*accelerationNeeded);
	double negativeThrottle = max(0, (1.0/10.0)*accelerationNeeded);
	double rawThrottle = clamp(positiveThrottle + negativeThrottle, -1.0,  1.0);

	double validatedThrottle = max(0.0, rawThrottle * 3.4323432343) - max(0.0, -10.0 * rawThrottle);
	printf("validatedThrottle %f | rawThrottle %f | positiveThrottle %f | negativeThrottle %f | accelerationNeeded %f\n", validatedThrottle, rawThrottle, positiveThrottle, negativeThrottle, accelerationNeeded);

	return validatedThrottle;
}


int main(){
	double throttleInput = computeThrottle(v, checkpointPositions, numCheckpoints, pos, forward);
	return 0;
}