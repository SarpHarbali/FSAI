#include <math.h>
#include <stdbool.h>
#include "physics.h"

// void UpdateState(State* state, Input* input, double dt);

double Clamp(double value, double min, double max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}
void ValidateState(State* state) {
    state->v_x = fmax(0, state->v_x);
};

void ValidateInput(VehicleModel model, Input* input) {
    double maxAcc = model.param.input_ranges.acc.max;
    double minAcc = model.param.input_ranges.acc.min;

    double maxVel = model.param.input_ranges.vel.max;
    double minVel = model.param.input_ranges.vel.min;

    double maxDelta = model.param.input_ranges.delta.max;
    double minDelta = model.param.input_ranges.delta.min;

    input->acc = Clamp(input->acc, minAcc, maxAcc);
    input->vel = Clamp(input->vel, minVel, maxVel);
    input->delta = Clamp(input->delta, minDelta, maxDelta);
};

double GetSlipAngle(VehicleModel model, State state, Input input, bool isFront) {
    double leverArmLength = model.param.kinematic.l * model.param.kinematic.w_front;
    
    if (!isFront) {
        double vX = fmax(1, state.v_x);
        return atan((state.v_y + leverArmLength)/(vX - 0.5 * model.param.kinematic.axle_width));
    }
    double vXFront = fmax(1, state.v_x);
    return atan((state.v_y + leverArmLength)/(vXFront - 0.5 * model.param.kinematic.axle_width)) - input.delta;
}

// We are ommiting the implementation of GetWheelSpeeds since it is never referenced

// int main() {
//     Param dryConfig = {
//         {225, 9.81, 31.27, 898.1, 1097.6},
//         {1.58, 0.869, 0.711, 0.45, 1.4},
//         {1, 12.56, -1.38, 1.60, -0.58, 0.2525},
//         {1.9, 1},
//         {
//             {-10, 3.4323432343},
//             {0, 30.1964649875},
//             {-0.3665191, 0.3665191}
//         }
//     };
//     VehicleModel myVModel = {dryConfig};
//     return 0;
// }