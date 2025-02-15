These instructions should be executed in the simulation directory
1. On the first time you want to run the program, you should run `./init.sh`.
2. On every reset of the system, run `./setup.sh`. This initialises the (virtual) can devices.
3. Start the main program with `./main vcan0`, which will show a data dashboard. In production, `./main can0` should be used
4. Run the VCU (vehicle control unit) program with `vcu/.vcu <device>` using the same device as in step 3. in parallel. This is required to select the mission. Input the mission as an integer according to the VCU2AI_Status.AMI_STATE in the [[ADS-DV_Software_Interface_Specification_v4.0.pdf]]