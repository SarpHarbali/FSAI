#include <math.h>
#include <stdio.h>      

double lookaheadSensitivity = 0.7;

// Positive angles are counterclockwise
double signedAngle(double a[], double b[]){
    double dotProduct = a[0] * b[0] + a[1] * b[1];
    double crossProduct = a[0] * b[1] - a[1] * b[0];
    double theta = atan2(crossProduct, dotProduct);
    return theta*180/M_PI;
}

// Returns the steering angle for CAN in the interval [-21, 21]°
double calculateSteeringAngle(double checkpoints[][3], int numberOfCheckpoints, double speed, double position[], double forward[], double deltaTime) {
    int lookaheadIndex = fmin(floor(lookaheadSensitivity * speed) + 1,
     numberOfCheckpoints - 1);
    double direction[] = {checkpoints[lookaheadIndex][0] - position[0],
    checkpoints[lookaheadIndex][2] - position[2]};
    double distance = sqrt(direction[0]*direction[0] + direction[1]*direction[1]);
    double theta = signedAngle(forward, direction);
    if (speed == 0) {
        speed = 0.1;
    }
    double reactionTime = fmax(distance/speed, 0.1);
    double deltaTheta = -theta/reactionTime;
    double updatedTheta = theta + deltaTheta*deltaTime;
    if (updatedTheta < -21) {
        updatedTheta = -21;
    } else if (updatedTheta > 21) {
        updatedTheta = 21;
    }
    return updatedTheta;
}

int main() {
    double a[2] = {2, 2};
    double b[2] = {-2, -1.9};
    printf("%f", signedAngle(a, b));
    return(0);
}