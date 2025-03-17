#include <math.h>
#include <stdio.h>
#include "physics.h"
#include "dynamic_bicycle.c"
#include "../controller_c/src/algorithm.c"

double Magnitude3(double x, double y, double z) {
    return sqrt(x*x + y*y + z*z);
}

int main() {
    // double x, y, z, yaw, v_x, v_y, v_z, r_x, r_y, r_z, a_x, a_y, a_z;
    State vState = {4.62, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Param dryConfig = {
    {225, 9.81, 31.27, 898.1, 1097.6},
    {1.58, 0.869, 0.711, 0.45, 1.4},
    {1, 12.56, -1.38, 1.60, -0.58, 0.2525},
    {1.9, 1},
    {
        {-10, 3.4323432343},
        {0, 30.1964649875},
        {-0.3665191, 0.3665191}
    }
    };
    VehicleModel vModel = {dryConfig};
    int numberOfCheckpoints = 4;
    Vector3 checkpointPositions[4] = {
        {3.92, 0.00, 0.19},
        {3.92, 0.00, 1.19},
        {3.92, 0.00, 2.19},
        {3.92, 0.00, 3.19},
    };
    printf("INITIALLY:\n%s\n", stringifyState(vState));
    int i = 0;
    while (i < 100) {
        double speed = Magnitude3(vState.v_x, vState.v_y, vState.v_z);
        Vector2 forward = {vState.r_z, -vState.r_x};
        Vector3 position = {vState.x, vState.y, vState.z};
        Input vInput = {
            calculateSteeringAngle(checkpointPositions, numberOfCheckpoints, speed, position, forward),
            0, calculateThrottle(checkpointPositions, numberOfCheckpoints, speed, position, forward)};
        ValidateInput(vModel, &vInput);
        UpdateState(vModel, &vState, vInput, DELTA_TIME);
        printf("%s\n", stringifyState(vState));
        i++;
    }
    // printf("\n==========================\nCalculated Steering Angle: %f\n", calculateSteeringAngle(checkpointPositions, numberOfCheckpoints, speed, position, forward));
    // printf("\n==========================\nCalculated Throttle: %f\n\n", calculateThrottle(checkpointPositions, numberOfCheckpoints, speed, position, forward));
    return 0;

}