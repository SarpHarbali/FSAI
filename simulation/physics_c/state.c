#include <stdio.h>
#include "physics.h"

State multiplyState(State state, double dt) {
    State newState = {
        state.x * dt,
        state.y * dt,
        state.z * dt,
        state.yaw * dt,
        state.v_x * dt,
        state.v_y * dt,
        state.v_z * dt,
        state.r_x * dt,
        state.r_y * dt,
        state.r_z * dt,
        state.a_x * dt,
        state.a_y * dt,
        state.a_z * dt
    };
    return newState;
}

State addStates(State stateA, State stateB) {
    State newState = {
        stateA.x   + stateB.x,
        stateA.y   + stateB.y,
        stateA.z   + stateB.z,
        stateA.yaw + stateB.yaw,
        stateA.v_x + stateB.v_x,
        stateA.v_y + stateB.v_y,
        stateA.v_z + stateB.v_z,
        stateA.r_x + stateB.r_x,
        stateA.r_y + stateB.r_y,
        stateA.r_z + stateB.r_z,
        stateA.a_x + stateB.a_x,
        stateA.a_y + stateB.a_y,
        stateA.a_z + stateB.a_z
    };
    return newState;
}

char* stringifyState(State state) {
    static char str[300];
    sprintf(str, "x: %f | y: %f | z: %f | yaw: %f | v_x: %f | v_y: %f | v_z: %f | r_x: %f | r_y: %f | r_z: %f | a_x: %f | a_y: %f | a_z: %f", 
    state.x, state.y, state.z, state.yaw, 
    state.v_x, state.v_y, state.v_z, 
    state.r_x, state.r_y, state.r_z,
    state.a_x, state.a_y, state.a_z);
    return str;
}

