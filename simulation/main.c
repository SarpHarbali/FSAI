#include <stdio.h>

#include <unistd.h>

#include <stdbool.h>

#include "./FS-AI_API/fs-ai_api.h"

void static_inspection_a() {
	fs_ai_api_vcu2ai vcu2ai_data;
	fs_ai_api_ai2vcu ai2vcu_data;
	ai2vcu_data.AI2VCU_ESTOP_REQUEST = 0;
	// TODO: Implement proper start-up sequence inline with the state machine
	
	int phase = 0;	
	float steer_angle = 0.0; 
	float steer_rate = 21.0; // in degrees per second
	float delta_theta = steer_rate/100;
	float rpm = 0.0;
	float delta_rpm = 0.2; // We need get rpm to 200 in 10 seconds. The Tx of the CAN message is 10ms.
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
		
		ai2vcu_data.AI2VCU_MISSION_STATUS = 1;
		switch (phase) {
			case 0:
				steer_angle += delta_theta;
				if (steer_angle >= 21) {steer_angle = 21; phase++;}
				break;
			case 1:
				steer_angle -= delta_theta;
				if (steer_angle <= -21) {steer_angle = -21; phase++;}
				break;
			case 2:
				steer_angle += delta_theta;
				if (steer_angle >= 0) {steer_angle = 0; phase++;}
			     	break;
			case 3:
				steer_angle = 0;
				rpm += delta_rpm; 
				if (rpm >= 200) {rpm=200; phase++;}
				break;
			case 4:
				rpm = 0;
				brake_pct += delta_brake;
				if (brake_pct >= 100) {brake_pct = 100; phase++;}
				break;
			case 5:
				// TODO: Add propper CAN sign-off and mission completion
				break;
		}
		ai2vcu_data.AI2VCU_STEER_ANGLE_REQUEST_deg = steer_angle;
		ai2vcu_data.AI2VCU_AXLE_SPEED_REQUEST_rpm = rpm;
		ai2vcu_data.AI2VCU_BRAKE_PRESS_REQUEST_pct = brake_pct;
		fs_ai_api_ai2vcu_set_data(&ai2vcu_data);
		usleep(10000);
	}	

};

int main(int argc, char** argv) {
while(1) {
    	if (argc < 2) {
    	printf("Too few arguments!\r\n");
	printf("Usage: fs-ai_api_tester <can>\r\n");
        return(1);
    }
        
    if(fs_ai_api_init(argv[1],1,0)) {	// initialise with Debug flag
	printf("fs_ai_api_init() failed\r\n");
	return(1);
    } 
	
    if(HANDSHAKE_RECEIVE_BIT_OFF == vcu2ai_data.VCU2AI_HANDSHAKE_RECEIVE_BIT) {
			ai2vcu_data.AI2VCU_HANDSHAKE_SEND_BIT = HANDSHAKE_SEND_BIT_OFF;
		} else if (HANDSHAKE_RECEIVE_BIT_ON == vcu2ai_data.VCU2AI_HANDSHAKE_RECEIVE_BIT) {
			ai2vcu_data.AI2VCU_HANDSHAKE_SEND_BIT = HANDSHAKE_SEND_BIT_ON;
		} else {	// should not be possible
			printf("HANDSHAKE_BIT error\r\n");
		}
		ai2vcu_data.AI2VCU_DIRECTION_REQUEST = 0;

    printf("Type a mission number: \n");
    int mission;
    scanf("%i", &mission);

    switch (mission) {
	case -1:
	    return(0);
        case 5:
            static_inspection_a();
            break;
        default:
	    printf("Mission not implemented\n");
            break;

    }
  }
}
