#include <stdio.h>

#include <unistd.h>

#include <stdbool.h>

#include "../FS-AI_API/fs-ai_api.h"

#include "../FS-AI_API/can.h"

static struct can_frame VCU2AI_Status = {0x520, 8};

int main(int argc, char** argv) {
   	if (argc < 2) {
    	printf("Too few arguments!\r\n");
	    printf("Usage: vcu <can>\r\n");
        return(1);
    }
    printf("%i", can_init(argv[1]));

    int mission;
    printf("Select mission number: \n");
    scanf("%i", &mission);

while(1) {
        
    VCU2AI_Status.data[2] = (uint8_t)(mission << 4);
    can_send(&VCU2AI_Status);
       
  }
}
