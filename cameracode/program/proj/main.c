#include "p30f6014A.h"
#include "stdio.h"
#include "string.h"

#include "uart/e_uart_char.h"
#include "motor_led/e_init_port.h"
#include "motor_led/e_epuck_ports.h"
#include "motor_led/advance_one_timer/e_motors.h"
#include "motor_led/advance_one_timer/e_agenda.h"

#include "imageCapture.h"
#include "findRed.h"
#include "followGreen.h"

int getselector() {
	return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}

int main() {
	int selector;
//init
	e_init_port();
	e_init_uart1();
	e_init_motors();

	selector=getselector();
	
	if (selector==0) {

	} else if (selector==1) {
		imageCapture();
	} else if (selector==2) {
		findRed();
	} else if (selector==3) {
		followGreen();
	} else if (selector==4) {

	} else if (selector==5) {

	} else {
	}

	
	while(1);
}

