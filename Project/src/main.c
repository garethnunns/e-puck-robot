#include "p30f6014A.h"
#include "stdio.h"
#include "string.h"

#include "../lib/motor_led/e_init_port.c"
#include "../lib/utility.c"

#define PI 3.14159265358979

int main() {
	char buffer[80];
	int selector;

	//system initialization 
	e_init_port();

	// Decide mode
	selector=getselector();
	
	if (selector==0) {
		//
	} else if (selector==1) {
		// Aggressive
	} else if (selector==2) {
		// Fear
	} else if (selector==3) {
		// Love
	} else if (selector==4) {
		// Curious
	} else if (selector==5) {
		// Goal Seeking and Obstacle Avoidance
	} else if (selector==6) {
		// High Level
	} else { 
		// flash all the lights?
		long i;
		int led=0;
		while(1)
				{
				LED0 = led = led^1;
				for(i=0;i<300000;i++)
					asm("nop");
		}
	}	
	while(1);
	return 0;
}
