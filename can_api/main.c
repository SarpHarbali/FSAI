#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>

#include "fs-ai_api.h"

void static_inspection_a() {
	fs_ai_api_vcu2ai vcu2ai_data;
	fs_ai_api_ai2vcu ai2vcu_data;
	// TODO: Implement proper start-up sequence inline with the state machine
	
	int phase = 0;	
    int moment = 0;
    float power = 17000; //this is the peak continuous 
    float PI = 3.141592;
  
	float rpm = 0.0;
	float delta_rpm = 0.4; // We need get rpm to 200 in 10 seconds. The Tx of the CAN message is 10ms.
	float brake_pct = 0.0;
	float delta_brake = 30.0/100;

	while(1) {
		printf("\e[1;1H\e[2J");	//clear screen
		printf("\033[H"); // home the cursor
		printf("AI2VCU_DIRECTION_REQUEST           %u    \r\n",ai2vcu_data.AI2VCU_DIRECTION_REQUEST);
		printf("AI2VCU_ESTOP_REQUEST               %u    \r\n",ai2vcu_data.AI2VCU_ESTOP_REQUEST);
		printf("AI2VCU_MISSION_STATUS              %u    \r\n",ai2vcu_data.AI2VCU_MISSION_STATUS);
		printf("AI2VCU_STEER_ANGLE_REQUEST_deg %+5.1f    \r\n",ai2vcu_data.AI2VCU_STEER_ANGLE_REQUEST_deg);
		printf("AI2VCU_AXLE_SPEED_REQUEST_rpm   %4.0f    \r\n",ai2vcu_data.AI2VCU_AXLE_SPEED_REQUEST_rpm);
		printf("AI2VCU_AXLE_TORQUE_REQUEST_Nm   %4.0f    \r\n",ai2vcu_data.AI2VCU_AXLE_TORQUE_REQUEST_Nm);
		printf("AI2VCU_BRAKE_PRESS_REQUEST_pct  %4.0f    \r\n",ai2vcu_data.AI2VCU_BRAKE_PRESS_REQUEST_pct);
        printf("Moment %d \r\n", moment);
        ai2vcu_data.AI2VCU_ESTOP_REQUEST = 0;	
		ai2vcu_data.AI2VCU_MISSION_STATUS = 1;
		ai2vcu_data.AI2VCU_DIRECTION_REQUEST = 0;
        ai2vcu_data.AI2VCU_AXLE_TORQUE_REQUEST_Nm = 95; // this keeps the motor at continues torque rating
		double steer_angle;

        switch (phase) {
			case 0:
				steer_angle = 21*sin(PI/2*moment*0.01);
				if (moment >= 400) {steer_angle = 0; phase++;}
				break;
			case 1:
				steer_angle = 0;
				rpm += delta_rpm; 
				if (rpm >= 200) {rpm=200; phase++;}
				break;
			case 2:
				rpm = 0;
				brake_pct += delta_brake;
				if (brake_pct >= 100) {brake_pct = 100; phase++;}
				break;
			case 3:
				// TODO: Add propper CAN sign-off and mission completion
				break;
		}
		ai2vcu_data.AI2VCU_STEER_ANGLE_REQUEST_deg = steer_angle;
		ai2vcu_data.AI2VCU_AXLE_SPEED_REQUEST_rpm = rpm;
		ai2vcu_data.AI2VCU_BRAKE_PRESS_REQUEST_pct = brake_pct;
        fs_ai_api_ai2vcu_set_data(&ai2vcu_data);
        moment++;
        usleep(10000);   // sleep for ~10ms
	}	

};

int main(int argc, char** argv) {

	fs_ai_api_vcu2ai vcu2ai_data;
	fs_ai_api_ai2vcu ai2vcu_data;

	if (argc < 2) {
	  	printf("Too few arguments!\r\n");
		printf("Usage: fs-ai_api_tester <can>\r\n");
		return(1);
		}
	        
    if(fs_ai_api_init(argv[1],1,0)) {	// initialise with Debug flag
		printf("fs_ai_api_init() failed\r\n");
		return(1);
    }


	while(1) {


	    
		fs_ai_api_vcu2ai_get_data(&vcu2ai_data); 

		if(HANDSHAKE_RECEIVE_BIT_OFF == vcu2ai_data.VCU2AI_HANDSHAKE_RECEIVE_BIT) {
				ai2vcu_data.AI2VCU_HANDSHAKE_SEND_BIT = HANDSHAKE_SEND_BIT_OFF;
			} else if (HANDSHAKE_RECEIVE_BIT_ON == vcu2ai_data.VCU2AI_HANDSHAKE_RECEIVE_BIT) {
				ai2vcu_data.AI2VCU_HANDSHAKE_SEND_BIT = HANDSHAKE_SEND_BIT_ON;
			} else {	// should not be possible

				printf("HANDSHAKE_BIT error\r\n");

			}
		
	
	    int mission;
	   	mission = vcu2ai_data.VCU2AI_AMI_STATE;
	   	//printf("\r Mission value is %d \n ", mission);
	    ai2vcu_data.AI2VCU_MISSION_STATUS = 1;
	    switch (mission) {
		case -1:
		    return(0);
		case 0:
			printf("\rMission not selected, mission value is %d", mission);
			fflush(stdout);
			usleep(100000);
			break;
	    case 5:
	        static_inspection_a();
	        break;
	    default:
		    printf("Mission not implemented \n");

	    }
	}
}
