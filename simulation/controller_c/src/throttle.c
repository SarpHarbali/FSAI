#include <math.h>
#include <stdio.h>
#include <stdbool.h>      

//code has lots of errors as just translated from c sharp. Will sort.

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
//to implement: feed sine wave into throttle
//physics model + track generation
//port simulationmodels folder
//controllers on pause

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
	float x, y;
} Vector2;



typedef struct {
    float w, x, y, z;
} Quaternion;

// positions of checkpoints, velocity, foward direction, direction to checkpoint at lookahead index - passed in from unity.

//values for lookahead
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

Vector3 transformPosition = {
	{2.20,0.56,0.29}
};

//values for throttle
double speedLookAheadDistance = 0;
float speedLookAheadSensitivity = 0.7;
float positiveThrottle = 0;
float negativeThrottle = 0;
int steeringLookAheadDistance = 0;
float steeringLookAheadSensitivity = 0.1;
float epsilon = 0.000001;

//Magnitude of vector
int Magnitude3 (Vector3 vector3){
	float x = vector3.x;
	float y = vector3.y;
	float z = vector3.z;
	return sqrt((x*x)+(y*y)+(z*z));
}

int Magnitude2 (Vector2 vector2){
	float x = vector2.x;
	float y = vector2.y;
	return sqrt((x*x)+(y*y));
}

// Vector2 Normalized2(Vector2 *vector){
// 	int magnitude = Magnitude2(*vector);
// 	if (magnitude>0){
// 		float x = vector.x/magnitude;
// 		float y /= vector.y/magnitude;
// 		return (Vector2){x, y}
// 	}
// 	else{
// 		return (Vector2){0,0};
// 	}
// }

//signed vector2 angle
float signedAngle2(Vector2 a, Vector2 b) {
    float dot = a.x * b.x + a.y * b.y;         
    float det = a.x * b.y - a.y * b.x;         
    float angle = atan2(det, dot);            
    return angle * (180.0f / M_PI);            
}

float clamp(float d, float min, float max) {
  const float t = d < min ? min : d;
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
int PrecomputeLookaheadData(Vector3* checkpointPositions, int checkpointSize, int cachedDirectionsSize, float carStateVelocity){
	speedLookAheadDistance = floor(speedLookAheadSensitivity * (float) carStateVelocity) + 1;
	steeringLookAheadDistance = floor(steeringLookAheadSensitivity * (float) carStateVelocity)+1;
	int maxLookaheadIndex = MAX(speedLookAheadDistance, steeringLookAheadDistance);


	if (checkpointPositions != NULL && checkpointSize > 0){

		maxLookaheadIndex = MIN(maxLookaheadIndex, checkpointSize - 1);
		//initialise cached arrays if necessary
		if (cachedDirectionsSize == 0 || cachedDirectionsSize != maxLookaheadIndex + 1){
			Vector2 cachedDirectionsToCheckpoints = [maxLookaheadIndex + 1];
            cachedDistancesToCheckpoints = float(maxLookaheadIndex + 1);

		}
		printf("enter currentPosition as x,y,z");
		int x, y, z;
		scanf("%f,%f,%f", &x, &y, &z);
		Vector3 currentPosition = {x, y, z};
		//Precompute directions and distances
		for (int i = 0; i <= maxLookaheadIndex; i++)
            {
                Vector3 directionToCheckpoint = checkpointPositions[i] - currentPosition;
                directionToCheckpoint[i].y = 0;
                float distanceToCheckpoint = Magnitude3(distanceToCheckpoint);
                printf("DISTANCE TO CHECKPOINT IS, %f\n", distanceToCheckpoint);
                Vector2 direction2D = {directionToCheckpoint.x, directionToCheckpoint.z};
                cachedDirectionsToCheckpoints[i] = direction2D;
                cachedDistancesToCheckpoints[i] = distanceToCheckpoint;
            }
	}

	return maxLookaheadIndex;
}


//throttle calculation 

float CalculateThrottleInput(double carStateVelocity){
	//1. Computing angle and distance
	int lookaheadIndex = MIN(speedLookaheadDistance, Magnitude2(cachedDirectionsToCheckpoints) - 1); //1.1
	Vector2 directionToCheckpoint = cachedDirectionsToCheckpoints[lookaheadIndex]; //1.2
	//Vector2 carFowardDirection = GetCarForwardDirection()
	float angle = signedAngle2(carFowardDirection, directionToCheckpoint); //1.3
	//2. Expected Speed
	float expectedSpeed = CalculateExpectedSpeed(angle);
	float distanceToCheckpoint = cachedDirectionsToCheckpoints[lookaheadIndex];
	float speed = (float)carStateVelocity;

	if (approx(speed, 0f)){
		speed = 0.1f;
	}

	//3. Required acceleration
	float accelerationNeeded = (expectedSpeed - speed) / (distanceToCheckpoint/speed);


	if (approx(accelerationNeeded, 0f)){
		accelerationNeeded = 1f;
	}

	return CalculateThrottle(accelerationNeeded);
}

float CalculateExpectedSpeed(float angle){
	if (approx(angle, 0f))
	{
		return 30.1964649875f;
	}
	float expression = fabs(1f / (accelerationFactor * angle)); //implement acceleration factor
	return MIN(expression, 30.1964649875f);
}


float CalculateThrottle(float acceleration){
	//4. Raw Throttle Computation
	float positiveThrottle = MAX(0, (1/3.4323432343)*acceleration);
	float negativeThrottle = MAX(0, (1/10)*acceleration);
	return clamp(positiveThrottle + negativeThrottle, -1, 1);
}

float ValidateThrottle(float throttle){
	//5. Validating and Re-Scale the Throttle
	return MAX(0f, throttle * 3.4323432343f) - MAX(0f, -10f * throttle);
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
	printf("positive throttle is %f\n", CalculateThrottle(1));
	printf("maxLookaheadIndex is %d\n", PrecomputeLookaheadData(checkpointPositions, sizeof(checkpointPositions), sizeof(cachedDirectionsToCheckpoints)carStateVelocity));
	return 0;
}