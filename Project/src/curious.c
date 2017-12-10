#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/a_d/e_prox.h"

#include "curious.h"

int curious_threshold = 300;
int curious_threshold_2 = 200;

void curious(int selection){
	
	e_init_port();
	e_init_prox();
	e_init_motors();


	while(1){
	
		//start moving
		curious_move();
		//avoid_set_leds();
		aggressive_wait();
	}	

	
}

void curious_wait(int cycles){
	long j = 0;
   	for(j=0;j<cycles;j++)
   	{
    	j = j;
   	}
   	return 0;
}

void curious_investigate(void){

	int leftDetect = 1;
	int leftCounter = 0;
	int rightDetect = 1;
	int rightCounter = 0;

	while(leftDetect == 1){
		int front = curious_get_front();
		e_set_speed_left(-500);
		e_set_speed_right(500);
		curious_wait(10000);	
		if(front < curious_threshold_2) leftDetect = 0;
		leftCounter = leftCounter + 1;
	}

	int i;
	for(i = 0; i < leftCounter; i++){
		e_set_speed_left(500);
		e_set_speed_right(-500);
		curious_wait(10000);
	}

	while(rightDetect == 1){
		int front = curious_get_front();
		e_set_speed_left(500);
		e_set_speed_right(-500);
		curious_wait(10000);	
		if(front < curious_threshold_2) rightDetect = 0;
		rightCounter = rightCounter + 1;
	}

	int j;
	for(j = 0; j < rightCounter; j++){
		e_set_speed_left(-500);
		e_set_speed_right(500);
		curious_wait(10000);
	}
	

	
}

void curious_move(void){

	int front_1 = e_get_prox(7);
	int front_2 = e_get_prox(0);
	int front = (front_1 > front_2)? front_1 : front_2;

	int left = e_get_prox(6);
	int right = e_get_prox(1);

	int back_1 = e_get_prox(3);
	int back_2 = e_get_prox(4);
	int back = (back_1 > back_2) ? back_1 : back_2;

	e_led_clear();

	if(front > 300){
		curious_investigate();
		e_set_led(0,1);
	}else{
		curious_go_straight();
	}

}

int curious_get_front(void){
	int front_1 = e_get_prox(7);
	int front_2 = e_get_prox(0);
	int front = (front_1 > front_2)? front_1 : front_2;
	return front;
}

void curious_stop(void){
	e_set_speed_left(0);
	e_set_speed_right(0);
}

void curious_turn_right(void){
	e_set_speed_left(500);
	e_set_speed_right(-500);
}

void curious_turn_left(void){
	e_set_speed_left(-500);
	e_set_speed_right(500);
}	

void curious_go_straight(void){
	e_set_speed_left(500);
	e_set_speed_right(500);
}

void curious_speed_up(void){
	e_set_speed_left(1000);
	e_set_speed_right(1000);
}

void curious_reverse(void){
	e_set_speed_left(-1000);
	e_set_speed_right(-1000);
}
