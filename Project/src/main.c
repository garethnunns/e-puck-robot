#include "p30f6014A.h"
#include "stdio.h"
#include "string.h"

//#include "../lib/codec/e_sound.h"
#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/uart/e_uart_char.h"
#include "../lib/a_d/e_ad_conv.h"
#include "../lib/a_d/e_prox.h"
//#include "..math.h"
#include "../lib/utility.h"

#define PI 3.14159265358979

/* our header files go here */
#include "highlevel.h"
#include "aggressive.h"
#include "avoid.h"
#include "main.h"

int main() {
	int selector;

	//system initialization 
	e_init_port();
	e_init_prox();
	e_init_motors();

	while(1){

		// Decide mode
		selector=getselector();
	
		//reset lights so they are off for all other modes
		e_led_clear();

		if (selector==0) {
			//
			avoid(selector);
		} else if (selector==1) {
			// Aggressive
			aggressive(selector);
		} else if (selector==2) {
			// Fear
			flash_led(selector);
		} else if (selector==3) {
			// Love
			flash_led(selector);
		} else if (selector==4) {
			// Curious
			flash_led(selector);
		} else if (selector==5) {
			// Goal Seeking and Obstacle Avoidance
			while(getselector() == 5){
				e_set_speed_left(500);
			}
		} else if (selector==6) {
			// High Level
			//runhighlevel();
			flash_led(selector);
		}else if (selector == 7){
			avoid(selector);
		}else { 
			flash_led(selector);
		}

	}
	return 0;
}


void flash_led(int selection){

	int led = 0;

	e_set_speed_left(0);
	e_set_speed_right(0);

	while(getselector() == selection){
		// flash the light
		led = (led >= 7)? 0 : led + 1;
		e_led_clear();
		e_set_led(led,1);
		long led_timer;
		for(led_timer=0;led_timer<250000;led_timer++){
			asm("nop");
		}
	}
}
