#include <math.h>
#include "physics.h"
#include "vehicle_model.c"

void UpdateState(State* state, Input* input, double dt) {
    ValidateInput(input);
    double Fz = GetNormalForce(*state);

        double slipAngleFront = GetSlipAngle(state, input, true);
        double FyF = GetFy(Fz, true, slipAngleFront);

        double slipAngleBack = GetSlipAngle(state, input, false);
        double FyR = GetFy(Fz, false, slipAngleBack);

        // Drivetrain Model
        double Fx = GetFx(state, input);
        // Dynamics
        State xDotDyn = _f(state, input, Fx, FyF, FyR);
        State xNextDyn = state + xDotDyn * dt;
        state = _fKinCorrection(xNextDyn, state, input, Fx, dt);

        // Set the acceleration based on the change in velocity
        state.a_x = xDotDyn.v_x;
        state.a_y = xDotDyn.v_y;

        ValidateState(ref state);
}