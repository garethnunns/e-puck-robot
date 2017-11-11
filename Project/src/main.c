#include "p30f6014A.h"
#include "stdio.h"
#include "string.h"

#include "uart/e_uart_char.h"
#include "motor_led/e_init_port.h"
#include "utility.h"

#define PI 3.14159265358979

int main() {
	char buffer[80];
	int selector;

	//system initialization 
	e_init_port();
	e_init_uart1();

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
	} else
		// flash all the lights?
	
	while(1);
	return 0;
}