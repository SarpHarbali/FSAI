#include <math.h>
#include <stdio.h>
#include <stdbool.h>      
#include "controller.h"

//code has lots of errors as just translated from c sharp. Will sort.

//to implement: feed sine wave into throttle
//physics model + track generation
//port simulationmodels folder
//controllers on pause

typedef struct {
    double w, x, y, z;
} Quaternion;

// positions of checkpoints, velocity, foward direction, direction to checkpoint at lookahead index - passed in from unity.

//values for throttle
double speedLookAheadDistance = 0;
double speedLookAheadSensitivity = 0.7;
double positiveThrottle = 0;
double negativeThrottle = 0;
int steeringLookAheadDistance = 0;
double steeringLookAheadSensitivity = 0.1;
double epsilon = 0.000001;

//Magnitude of vector
double Magnitude3 (Vector3 vector3){
	double x = vector3.x;
	double y = vector3.y;
	double z = vector3.z;
	return sqrt((x*x)+(y*y)+(z*z));
}

double Magnitude2 (Vector2 vector2){
	double x = vector2.x;
	double y = vector2.y;
	return sqrt((x*x)+(y*y));
}

// Vector2 Normalized2(Vector2 *vector){
// 	int magnitude = Magnitude2(*vector);
// 	if (magnitude>0){
// 		double x = vector.x/magnitude;
// 		double y /= vector.y/magnitude;
// 		return (Vector2){x, y}
// 	}
// 	else{
// 		return (Vector2){0,0};
// 	}
// }

//signed vector2 angle
double signedAngle2(Vector2 a, Vector2 b) {
    double dot = a.x * b.x + a.y * b.y;         
    double det = a.x * b.y - a.y * b.x;         
    double angle = atan2(det, dot);            
    return angle * (180.0 / M_PI);            
}

double clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;		
}

bool approx(int a, int b){
	if (a-b<epsilon | b-a<epsilon){
		return true;
	}
	else{
		return false;
	}
}


//LOOKAHEAD DISTANCE
int PrecomputeLookaheadData(Vector3* checkpointPositions, int checkpointSize, int cachedDirectionsSize, double carStateVelocity){
	speedLookAheadDistance = floor(speedLookAheadSensitivity * (double) carStateVelocity) + 1;

	if (checkpointPositions != NULL && checkpointSize > 0){

		maxLookaheadIndex = MIN(maxLookaheadIndex, checkpointSize - 1);
		//initialise cached arrays if necessary
		if (cachedDirectionsSize == 0 || cachedDirectionsSize != maxLookaheadIndex + 1){
			Vector2 cachedDirectionsToCheckpoints[maxLookaheadIndex + 1];
            double cachedDistancesToCheckpoints[maxLookaheadIndex + 1];

		}
		printf("enter currentPosition as x,y,z");
		int x, y, z;
		scanf("%d,%d,%d", &x, &y, &z);
		Vector3 currentPosition = {x, y, z};
		//Precompute directions and distances
		for (int i = 0; i <= maxLookaheadIndex; i++)
            {
                Vector3 directionToCheckpoint = {checkpointPositions[i].x - currentPosition.x,
					0, checkpointPositions[i].z - currentPosition.z};
                double distanceToCheckpoint = Magnitude3(directionToCheckpoint);
                printf("DISTANCE TO CHECKPOINT IS, %f\n", distanceToCheckpoint);
                Vector2 direction2D = {directionToCheckpoint.x, directionToCheckpoint.z};
                cachedDirectionsToCheckpoints[i] = direction2D;
                cachedDistancesToCheckpoints[i] = distanceToCheckpoint;
            }
	}

	return maxLookaheadIndex;
}


//throttle calculation 

double CalculateThrottleInput(double carStateVelocity){
	//1. Computing angle and distance
	int lookaheadIndex = MIN(speedLookAheadDistance, numberOfCheckpoints - 1); //1.1
	Vector2 directionToCheckpoint = cachedDirectionsToCheckpoints[lookaheadIndex]; //1.2
	//Vector2 carFowardDirection = GetCarForwardDirection()
	double angle = signedAngle2(carFowardDirection, directionToCheckpoint); //1.3
	//2. Expected Speed
	double expectedSpeed = CalculateExpectedSpeed(angle);
	double distanceToCheckpoint = cachedDirectionsToCheckpoints[lookaheadIndex];
	double speed = (double)carStateVelocity;

	if (approx(speed, 0)){
		speed = 0.1;
	}

	//3. Required acceleration
	double accelerationNeeded = (expectedSpeed - speed) / (distanceToCheckpoint/speed);


	if (approx(accelerationNeeded, 0)){
		accelerationNeeded = 1;
	}

	return CalculateThrottle(accelerationNeeded);
}

double CalculateExpectedSpeed(double angle){
	if (approx(angle, 0))
	{
		return 30.1964649875;
	}
	double expression = fabs(1 / (3.4323432343 * angle)); // implement acceleration factor
	return MIN(expression, 30.1964649875);
}


double CalculateThrottle(double acceleration){
	//4. Raw Throttle Computation
	double positiveThrottle = MAX(0, (1/3.4323432343)*acceleration);
	double negativeThrottle = MAX(0, (1/10)*acceleration);
	return clamp(positiveThrottle + negativeThrottle, -1, 1);
}

double ValidateThrottle(double throttle){
	//5. Validating and Re-Scale the Throttle
	return MAX(0, throttle * 3.4323432343) - MAX(0, -10 * throttle);
}

//GET DIRECTION FROM UNITY
Vector2 GetCarForwardDirection()
{
	//TRANSFORM OBJECT has position and rotation in one object
	//transform.forward.x view in unity
	//pass just the transform object as the argument?
	//add one more argument to getthrottleinput
	//need checkpoint velocity rotation object
	//list of global 3d locations of the cones
	printf("enter x,y \n");
	int x,y;
	scanf("%f,%f", &x,&y);
	Vector2 forwardDirection = {x, y};
	return forwardDirection;
}



int main(){
	double carStateVelocity = 10;
	Vector3 checkpointPositions[] = {
		{1.0, 2.0, 3.0},
		{4.0, 5.0, 6.0},
		{7.0, 8.0, 9.0}
	};

	Vector2 cachedDirectionsToCheckpoints[] = {
		{1.0, 2.0},
		{3.0, 4.0},
		{5.0, 6.0}
	};

	double cachedDistancesToCheckpoints[] = {
		3, 2, 1
	};

	Vector3 transformPosition = {2.20,0.56,0.29};	
	printf("positive throttle is %f\n", CalculateThrottle(1));
	printf("maxLookaheadIndex is %d\n", PrecomputeLookaheadData(checkpointPositions, sizeof(checkpointPositions), sizeof(cachedDirectionsToCheckpoints), carStateVelocity));
	return 0;
}