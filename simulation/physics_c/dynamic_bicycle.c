#include <math.h>
#include "physics.h"
#include "vehicle_model.c"
#include "state.c"

double Magnitude2(double x, double y) {
    return sqrt(x*x + y*y);
}

double GetFdrag(VehicleModel vModel, State x) {
    return vModel.param.aero.c_drag * x.v_x * x.v_x;
}

double GetFdown(VehicleModel vModel, State x) {
    return vModel.param.aero.c_down * x.v_x * x.v_x;
}

double GetDownForceFront(VehicleModel vModel, double Fz) {
    double FzAxle = 0.5 * vModel.param.kinematic.w_front * Fz;
    return FzAxle;
}

double GetDownForceRear(VehicleModel vModel, double Fz) {
    double FzAxle = 0.5 * (1 - vModel.param.kinematic.w_front) * Fz;
    return FzAxle;
}

double GetFy(VehicleModel vModel, double Fz, bool front, double slipAngle) {
    double FzAxle = front ? GetDownForceFront(vModel, Fz) : GetDownForceRear(vModel, Fz);
    double B = vModel.param.tire.B;
    double C = vModel.param.tire.C;
    double D = vModel.param.tire.D;
    double E = vModel.param.tire.E;
    double muY = D * sin(C*atan(B*(1-E)*slipAngle + E*atan(B*slipAngle)));
    double Fy = FzAxle * muY;
    return Fy;
}

double GetFx(VehicleModel vModel, State x, Input u) {
    double acc = x.v_x <= 0.0 && u.acc < 0.0 ? 0.0 : u.acc;
    double Fx = acc * vModel.param.inertia.m - GetFdrag(vModel, x);
    return Fx;
}

double GetNormalForce(VehicleModel vModel, State x) {
    return vModel.param.inertia.g * vModel.param.inertia.m + GetFdown(vModel, x);
}

State ComputeDerivative(VehicleModel vModel, State x, Input u, double Fx, double FyF, double FyR) {
        double FyFTot = 2.0 * FyF;
        double FyRTot = 2.0 * FyR;

        double _x = cos(x.yaw) * x.v_x - sin(x.yaw) * x.v_y;
        double _y = sin(x.yaw) * x.v_x + cos(x.yaw) * x.v_y;
        double yaw = x.r_z;
        double v_x = (x.r_z * x.v_y) + (Fx - sin(u.delta) * FyFTot) / vModel.param.inertia.m;
        double v_y = ((cos(u.delta) * FyFTot) + FyRTot) / vModel.param.inertia.m - (x.r_z * x.v_x);
        double r_z = (cos(u.delta) * FyFTot * vModel.param.kinematic.l_F - FyRTot * vModel.param.kinematic.l_R) / vModel.param.inertia.I_z;

        State xDot = {_x, _y, 0, yaw, v_x, v_y, 0, 0, 0, r_z, 0, 0, 0};
        return xDot;
}

State CorrectKinematics(VehicleModel vModel, State xIn, State xState, Input u, double Fx, double dt) {
    State x = xIn;
    double vXDot = Fx / vModel.param.inertia.m;
    double v = Magnitude2(xState.v_x, xState.v_y);
    double vBlend = 0.5 * (v - 1.5);
    double blend = Clamp(vBlend, 0, 1);
    x.v_x = blend * x.v_x + (1 - blend) * (xState.v_x + dt * vXDot);
    double vY = tan(u.delta) * x.v_x * vModel.param.kinematic.l_R / vModel.param.kinematic.l;
    double r = tan(u.delta) * x.v_x / vModel.param.kinematic.l;
    x.v_y = blend * x.v_y + (1 - blend) * vY;
    x.r_z = blend * x.r_z + (1 - blend) * r;
    return x;
}

void UpdateState(VehicleModel vModel, State* state, Input input, double dt) {
    ValidateInput(vModel, &input);
    double Fz = GetNormalForce(vModel, *state);
    double slipAngleFront = GetSlipAngle(vModel, *state, input, true);
    double FyF = GetFy(vModel, Fz, true, slipAngleFront);

    double slipAngleBack = GetSlipAngle(vModel, *state, input, false);
    double FyR = GetFy(vModel, Fz, false, slipAngleBack);

    // Drivetrain Model
    double Fx = GetFx(vModel, *state, input);
    // Dynamics
    State xDotDyn = ComputeDerivative(vModel, *state, input, Fx, FyF, FyR);
    State xNextDyn = multiplyState(addStates(*state, xDotDyn), dt);
    *state = CorrectKinematics(vModel, xNextDyn, *state, input, Fx, dt);

    // Set the acceleration based on the change in velocity
    state->a_x = xDotDyn.v_x;
    state->a_y = xDotDyn.v_y;

    ValidateState(state);
}