#ifndef STATE_H
#define STATE_H

typedef struct {
    double x, y, z, yaw, v_x, v_y, v_z, r_x, r_y, r_z, a_x, a_y, a_z;
} State;

#endif

#ifndef INPUT_H
#define INPUT_H

typedef struct {
	double acc, vel, delta;
} Input;

#endif

#ifndef INERTIA_H
#define INERTIA_H

typedef struct{
    double m,g,I_z,C_f,C_r;
} Inertia;

#endif

#ifndef KINEMATIC_H
#define KINEMATIC_H

typedef struct{
	double l, b_F, b_R, w_front, l_F, l_R, axle_width;
} Kinematic;

#endif

#ifndef TIRE_H
#define TIRE_H

typedef struct{
	double tire_coefficient, B, C, D, E, radius;
} Tire;

#endif

#ifndef AERO_H
#define AERO_H

typedef struct{
	double c_down, c_drag;
} Aero;

#endif

#ifndef RANGE_H
#define RANGE_H

typedef struct{
		double min, max;
} Range;

#endif

#ifndef INPUTRANGES_H
#define INPUTRANGES_H

typedef struct{
	Range acc, vel, data;
} InputRanges;

#endif

#ifndef PARAM_H
#define PARAM_H

typedef struct {		
	Inertia inertia;
	Kinematic kinematic;
	Tire tire;
	Aero aero;
	InputRanges input_ranges;
} Param;

#endif
// Not defining WheelInfo as it is not used