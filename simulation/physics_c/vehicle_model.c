#include <math.h>
#include "physics.h"

// void UpdateState(State* state, Input* input, double dt);

double Clamp(double value, double min, double max) {
    if (value < min) {
        return min
    } else if (value > max) {
        return max
    } else {
        return value
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
    return atan((state.v_y + leverArmLength)/(vX - 0.5 * model.param.kinematic.axle_width)) - input.delta;
}

// We are ommiting the implementation of GetWheelSpeeds since it is never referenced

// int main() {
//     Param myParam = {}
//     VehicleModel myVModel = {}
//     return 0
// }