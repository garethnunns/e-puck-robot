#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/a_d/e_prox.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

long park_space_counter = 0;
int park_stop = 0;
long park_back_counter = 0;
long returned_steps = 0;
long park_leftSteps = 0;
long park_right_steps = 0;

void park(int value){

	e_init_prox();

	//go along wall
	//start counter when right sensor has no value
	//stop counter when value again
	//calculate space size
	//if big enough go back
	//turn right 90 degrees
	//go forward until sensor value
	//wait 2000000 ish
	//turn left 90 degrees

	park_go_straight();	
	
	while(park_stop == 0){
		park_scan_gap();
	}


}

void park_turnLightsOn(){
	// should be an infinite wait that turns lights on, called when goal is found
	e_set_speed_left(0);
	e_set_speed_right(0);
	e_set_led(8,1);
	park_wait(10000);
	while(1){
		asm("nop");
		park_wait(10000);
	}
}

void park_scan_gap(){

	int park_check = park_check_sensor();
	if(park_check == 1){
		
		if(park_space_counter > 700){
			e_set_led(0,1);
			park_back_counter = park_space_counter/2;
			e_set_steps_right(0);
			park_reverse();
		}
		e_set_steps_left(0);

	}
	else{
		park_space_counter = e_get_steps_left();

		//e_set_led(0,0);
	}
}

void park_reverse(){

	e_set_led(0,0);
	if(returned_steps < park_back_counter){
		e_set_speed_left(-500);
		e_set_speed_right(-500);
		park_wait(10000);
		returned_steps = labs(e_get_steps_right());
		park_reverse();
	} else {
		e_set_speed_left(0);
		e_set_speed_right(0);
		park_turn_right_90_degrees();
		park_go_straight();
		park_wait(1800000);
		park_turn_left_90_degrees();
		park_turnLightsOn();
	}
}

void park_turn_right_90_degrees(){
	e_set_steps_left(0); // reset left steps
	e_set_led(0,0);
	do{
		e_set_led(4,1);	 	
		e_set_speed_left(315);
		e_set_speed_right(-315);
		park_leftSteps = e_get_steps_left();
		park_wait(10000);
	}while(park_leftSteps<=315);
	e_set_led(4,0);  		   
}

 void park_turn_left_90_degrees(){
	e_set_steps_right(0); // reset left steps
	e_set_led(0,0);
	do{
		e_set_led(4,1);	 	
		e_set_speed_left(-315);
		e_set_speed_right(315);
		park_right_steps = e_get_steps_right();
		park_wait(10000);
	}while(park_right_steps<=315);
	e_set_led(4,0);  		   
}

void park_wait(long value)
{
   long a;
   for(a=0;a<value;a++)
   {
       	asm("nop");
   }
}

void park_go_straight(){

	e_set_speed_left(500);
	e_set_speed_right(500);

}

int park_check_sensor(){

	int prox;
	prox = e_get_prox(2);
	if(prox > 300){
		e_set_led(2,0);
		return 1;
	}else{
		e_set_led(2,1);
		return 0;
	}
}