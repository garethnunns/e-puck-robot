#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/a_d/e_prox.h"

#include "fear.h"

void fear(int selection){
	
	e_init_port();
	e_init_prox();
	e_init_motors();

	while(1){
	
		//start moving
		fear_move();
		//avoid_set_leds();
		aggressive_wait();
	}	

	
}

void fear_set_leds(void){
	
	int i;

	for(i=0; i<8; i++){
		if(e_get_prox(i) > 200){
			e_set_led(i,1);
		}else{
			e_set_led(i,0);
		}
	}
	
}

void fear_move(void){

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
		fear_reverse();
		e_set_led(0,1);
	}else if(front > 200 && right > left){
		e_set_speed_left(-500);
		e_set_speed_right(-1000);
		e_set_led(0,1);
		e_set_led(1,1);
	}else if(front > 200 && left > right){
		e_set_speed_left(-1000);
		e_set_speed_right(-500);
		e_set_led(0,1);
		e_set_led(7,1);
	}else if(left > 200){
		e_set_speed_left(-1000);
		e_set_speed_right(-500);
		e_set_led(7,1);
	}else if(right > 200){
		e_set_speed_left(-500);
		e_set_speed_right(-1000);
		e_set_led(1,1);
	}else if(back > 200){
		fear_speed_up();
		e_set_led(4,1);
	}else{
		fear_stop();
	}

}

void fear_stop(void){
	e_set_speed_left(0);
	e_set_speed_right(0);
}

void fear_turn_right(void){
	e_set_speed_left(500);
	e_set_speed_right(-500);
}

void fear_turn_left(void){
	e_set_speed_left(-500);
	e_set_speed_right(500);
}	

void fear_go_straight(void){
	e_set_speed_left(500);
	e_set_speed_right(500);
}

void fear_speed_up(void){
	e_set_speed_left(1000);
	e_set_speed_right(1000);
}

void fear_reverse(void){
	e_set_speed_left(-1000);
	e_set_speed_right(-1000);
}
