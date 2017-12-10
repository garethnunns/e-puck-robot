#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/a_d/e_prox.h"

#include "love.h"

void love(int selection){
	
	e_init_port();
	e_init_prox();
	e_init_motors();

	while(1){
	
		//start moving
		love_move();
		//love_set_leds();
		aggressive_wait();
	}	

	
}

void love_set_leds(void){
	
	int i;

	for(i=0; i<8; i++){
		if(e_get_prox(i) > 200){
			e_set_led(i,1);
		}else{
			e_set_led(i,0);
		}
	}
	
}

void love_move(void){

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
		love_reverse();
	}	
	else if(front > 200){
		love_stop();
		e_set_led(0,1);
	}else{
		love_go_straight();
	}

}

void love_stop(void){
	e_set_speed_left(0);
	e_set_speed_right(0);
}

void love_turn_right(void){
	e_set_speed_left(500);
	e_set_speed_right(-500);
}

void love_turn_left(void){
	e_set_speed_left(-500);
	e_set_speed_right(500);
}	

void love_go_straight(void){
	e_set_speed_left(500);
	e_set_speed_right(500);
}

void love_speed_up(void){
	e_set_speed_left(1000);
	e_set_speed_right(1000);
}

void love_reverse(void){
	e_set_speed_left(-500);
	e_set_speed_right(-500);
}
