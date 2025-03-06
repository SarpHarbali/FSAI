#include "state.c"
#include "input.c"

int main() {
    Input myInput = {1,2,3};
    printf("%s", stringifyInput(myInput));
    State myState = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    State yourState = {1,2,3,4,5,6,7,8,9,10,11,12,13};
    myState = multiplyState(myState, 0.2);
    myState = addStates(myState, yourState);
    printf("%s", stringifyState(myState));
    return(0);
}