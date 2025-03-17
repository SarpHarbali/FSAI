#include <math.h>
#include <stdio.h>      
#include "controller.h"

double speedLookaheadSensitivity = 0.7;
double steeringLookaheadSensitivity = 0.1;
double DELTA_TIME = 0.01;
double MAX_SPEED = 30.1964649875;
double ACC_FACTOR =  0.002;
double EPSILON = 0.000001;
double THR_FACTOR = 3.4323432343;

// Positive angles are counterclockwise
double signedAngle(Vector2 a, Vector2 b){
    double dotProduct = a.x * b.x + a.y * b.y;
    double crossProduct = a.x * b.y - a.y * b.x;
    double theta = atan2(crossProduct, dotProduct)*180/M_PI;
    if (theta == -180.0f)
        theta = 180;
    return theta;
}

double clamp(double d, double min, double max) {
  const double t = d < min ? min : d;
  return t > max ? max : t;		
}

// Returns the steering angle for CAN in the interval [-21, 21]°
double calculateSteeringAngle(Vector3 checkpoints[], int numberOfCheckpoints, double speed, Vector3 position, Vector2 forward) {
    int lookaheadIndex = fmin(floor(steeringLookaheadSensitivity * speed) + 1,
     numberOfCheckpoints - 1);
    // printf("Lookahead Index: %d\n", lookaheadIndex);
    Vector2 direction = {checkpoints[lookaheadIndex].x - position.x,
    checkpoints[lookaheadIndex].z - position.z};
    // printf("Desired direction: (%f, %f)\n", direction.x, direction.y);
    double distance = sqrt(direction.x*direction.x + direction.y*direction.y);
    // printf("Distance: %f\n", distance);
    double theta = signedAngle(forward, direction);
    // printf("Angle: %f\n", theta);
    if (speed == 0) {
        speed = 0.1;
    }
    double reactionTime = fmax(distance/speed, 0.1);
    double deltaTheta = -theta/reactionTime;
    double updatedTheta = theta + deltaTheta*DELTA_TIME;
    if (updatedTheta < -21) {
        updatedTheta = -21;
    } else if (updatedTheta > 21) {
        updatedTheta = 21;
    }
    return updatedTheta;
}


double calculateThrottle(Vector3 checkpoints[], int numberOfCheckpoints, double speed, Vector3 position, Vector2 forward) {
        int lookaheadIndex = fmin(floor(speedLookaheadSensitivity * speed) + 1,
        numberOfCheckpoints - 1);
        // printf("Lookahead Index: %d\n", lookaheadIndex);
        Vector2 direction = {checkpoints[lookaheadIndex].x - position.x,
        checkpoints[lookaheadIndex].z - position.z};
        // printf("Desired direction: (%f, %f)\n", direction.x, direction.y);
        double distance = sqrt(direction.x*direction.x + direction.y*direction.y);
        // printf("Distance: %f\n", distance);
        double theta = signedAngle(forward, direction);
        // printf("Angle: %f\n", theta);
        double expectedSpeed;
        if (theta == 0) {
            expectedSpeed = MAX_SPEED;
        } else {
            expectedSpeed = fmin(fabs(1/(ACC_FACTOR * theta)), MAX_SPEED);
        }
        double desiredAccleration = ((expectedSpeed - speed)*speed)/distance;
        if (desiredAccleration<EPSILON && -EPSILON<desiredAccleration) {
            desiredAccleration = 1;
        }
        double rawThrottle = clamp(fmax(0, desiredAccleration/THR_FACTOR) + fmin(desiredAccleration/10, 0), -1, 1);
        double finalThrottle = fmax(0, rawThrottle*THR_FACTOR) - fmax(0, -10*rawThrottle);
        return rawThrottle;
}

// int main() {
//     int numberOfCheckpoints = 4;
//     Vector3 checkpointPositions[4] = {
//         {3.92, 0.00, 0.19},
//         {3.92, 0.00, 1.19},
//         {3.92, 0.00, 2.19},
//         {3.92, 0.00, 3.19},
//     };
//     double speed = 0.3;
//     Vector3 position = {5.92, 0.00, -0.2};  
//     Vector2 forward = {0, 1};  // Make an angle of 0 with the positive Y-axis according to Unity's starting rotation
//     printf("\n");
//     printf("\n==========================\nCalculated Throttle: %f\n", calculateThrottle(checkpointPositions, numberOfCheckpoints, speed, position, forward));
//     printf("\n==========================\nCalculated Steering Angle: %f\n", calculateSteeringAngle(checkpointPositions, numberOfCheckpoints, speed, position, forward));
//     return 0;
// }