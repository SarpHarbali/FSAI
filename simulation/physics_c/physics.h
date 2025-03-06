typedef struct {

} State;

typedef struct {

	double acc, vel, delta;

} Input;

typedef struct {
	typedef struct{
		double m,g,I_z,C_f,C_r;
	}
	Inertia;

	typedef struct{
		double l, b_F, b_R, w_front, l_F, l_R, axle_width;
	}
	Kinematic;

	typedef struct{
		double tire_coefficient, B, C, D, E, radius;
	}
	Tire;

	typedef struct{
		double c_down, c_drag;
	} Aero;

	typedef struct{
		typedef struct{
			double min, max;
		} Range;
		
	Range acc, vel, delta;
	}
	InputRanges;

	Inertia inertia;
	Kinematic kinematic;
	Tire tire;
	Aero aero;
	InputRanges input_ranges;
} Param;

// Not defining WheelInfo as it is not used