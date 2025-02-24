#include <math.h>
#include <stdio.h>      

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
//to implement: feed sine wave into throttle

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
	float x, y;
} Vector2;


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

//values for raw throttle
double speedLookAheadDistance = 0;
float speedLookAheadSensitivity = 0.7;
float positiveThrottle = 0;
float negativeThrottle = 0;
int steeringLookAheadDistance = 0;
float steeringLookAheadSensitivity = 0.1;

// int Magnitude3(float x, float y, float z){
	
// 	return sqrt((x*x)+(y*y)+(z*z));
// }

int Magnitude3 (Vector3 vector){
	float x = vector.x;
	float y = vector.y;
	float z = vector.z;
	return sqrt((x*x)+(y*y)+(z*z));
}

//IMPLEMENT FUNCTION FOR SIZEOF?? NEEDED? yes because keep changing

int length(Vector3 vectors[]){
	vectors[1]
}

//LOOKAHEAD DISTANCE
int PrecomputeLookaheadData(Vector3* checkpointPositions, int checkpointSize, int cachedDirectionsSize, float carStateVelocity){
	speedLookAheadDistance = floor(speedLookAheadSensitivity * (float) carStateVelocity) + 1;
	steeringLookAheadDistance = floor(steeringLookAheadSensitivity * (float) carStateVelocity)+1;
	int maxLookaheadIndex = MAX(speedLookAheadDistance, steeringLookAheadDistance);

	//sizeof returns size in memory, so divide by size of first element
	//unsigned long checkpointPositionsLength = check/sizeof(checkpointPositions[0]);
	//unsigned long cachedDirectionsToCheckpointLength = size/sizeof(cachedDirectionsToCheckpoints[0]);
	if (checkpointPositions != NULL && checkpointPositionsLength > 0){
		// printf("size of checkpointPositions is %d\n", size/(int)sizeof(checkpointPositions[0]));
		// printf("checkpointPositions calculation is %lu \n", sizeof(*checkpointPositions)/sizeof(checkpointPositions[0]));
		// printf("checkpointPositions isnt null and size is more than 0 \n");



		maxLookaheadIndex = MIN(maxLookaheadIndex, checkpointPositionsLength - 1);

		//initialise cached arrays if necessary
		//DIRECTIONS TO CHECKPOINT IMPLEMENT ABOVE, MAKE FUNCTION
		if (cachedDirectionsToCheckpoints == NULL || cachedDirectionsToCheckpointsLength != maxLookaheadIndex + 1){

			Vector2 cachedDirectionsToCheckpoints = [maxLookaheadIndex + 1];
            cachedDistancesToCheckpoints = float(maxLookaheadIndex + 1);

		}

		//where to get transform.position from? check vcu2ai
		Vector3 currentPosition = transform.position;
		//Precompute directions and distances
		for (int i = 0; i <= maxLookaheadIndex; i++)
            {
                Vector3 directionToCheckpoint = checkpointPositions[i] - currentPosition;
                directionToCheckpoint[i].y = 0;
                //float distanceToCheckpoint = sqrt((distanceToCheckpoint.x * distanceToCheckpoint.x) + (distanceToCheckpoint.y*distanceToCheckpoint.y)+(distanceToCheckpoint.z)*(distanceToCheckpoint.z))
                float distanceToCheckpoint = Magnitude3(distanceToCheckpoint);
                printf("DISTANCE TO CHECKPOINT IS, %f\n", distanceToCheckpoint);
                //Vector2 direction2D = new Vector2(directionToCheckpoint.x, directionToCheckpoint.z).normalized;

                //cachedDirectionsToCheckpoints[i] = direction2D;
                //cachedDistancesToCheckpoints[i] = distanceToCheckpoint;
            }


	}

	return maxLookaheadIndex;
}


//RAW THROTTLE COMPUTATION
float clamp(float d, float min, float max) {
  const float t = d < min ? min : d;
  return t > max ? max : t;
}

float CalculateThrottle(float acceleration){
	float positiveThrottle = MAX(0, (1/3.4323432343)*acceleration);
	float negativeThrottle = MAX(0, (1/10)*acceleration);
	return clamp(positiveThrottle + negativeThrottle, -1, 1);
}



int main(){
	printf("positive throttle is %f\n", CalculateThrottle(1));
	printf("maxLookaheadIndex is %d\n", PrecomputeLookaheadData(checkpointPositions, sizeof(checkpointPositions), sizeof(cachedDirectionsToCheckpoints)carStateVelocity));
	return 0;
}