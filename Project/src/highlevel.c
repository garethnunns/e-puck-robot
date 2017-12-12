#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_epuck_ports.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/uart/e_uart_char.h"
#include "../lib/a_d/e_ad_conv.h"
#include "../lib/a_d/e_prox.h"
#include "../lib/utility.h"
#include "../lib/camera/fast_2_timer/e_poxxxx.h"
#include "../lib/motor_led/advance_one_timer/e_led.h"

#include "time.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"


char high_buffer[160];
int high_numbuffer[80];
long high_isRedVisable;
int high_stop = 0;
int high_nospin = 0;


void high_follower(int selection){
	
	e_init_port();
	e_init_prox();
	e_init_motors();

	e_poxxxx_init_cam();
	e_poxxxx_config_cam(0,(ARRAY_HEIGHT - 4)/2,640,4,8,4,RGB_565_MODE);
	e_poxxxx_write_cam_registers();

	e_start_agendas_processing();

	int looking = 1;
	int following = 0;

	//main program loop for follower
	while(1){
		if(looking){
				int is_full_red = high_sense_green();
				int front_1 = e_get_prox(0);
				int front_2 = e_get_prox(7);

				if(is_full_red && (front_1 > 300 || front_2 > 300)){
						looking = 0;
						following = 1;
				}
		}

		if(following){
			if(high_prox_clear()){
				looking = 1;
				following = 0;
			}

			//turn and drive forward
		}

		high_wait(10000);
	}
}

void high_leader(int selection){
	
	e_init_port();
	e_init_prox();
	e_init_motors();

	int followed = 1;

	//main program loop for leader
	while(1){

		int back_1 = e_get_prox(3);
		int back_2 = e_get_prox(4);
		int back = (back_1 > back_2) ? back_1 : back_2;

		followed = (back > 150)? 1 : 0; 

		if(followed){		
			avoid_move();
			e_set_body_led(0);
		}else{
			e_set_speed_left(0);
			e_set_speed_right(0);		
			e_set_body_led(1);
		}	

		aggressive_wait();
	}


}

int high_sense_green(void){
	int centreValue;

	high_get_image();
	high_image();

	centreValue = high_numbuffer[38] + high_numbuffer[39] + high_numbuffer[40] + high_numbuffer[41] + high_numbuffer[42] + high_numbuffer[43]; // removes stray

	if(centreValue > 3) return 1;
	return 0;
}

//Check if all proximity sensors are clear
int high_prox_clear(void){
	int i;
	for(i = 0; i < 8; i++){
		if(e_get_prox(i) > 300) return 0;
	}
	return 1;
}


void high_drive_randomly(void){
	// this is when the robot is being followed it just drives semi-randomly
	//int random = high_randomNumber(6) - 3;

	// drive fairly slow so the follower can keep up (and turn)
	e_set_speed_left(500);
	e_set_speed_right(500);
}

int high_randomNumber (int max){
	time_t t;

	srand((unsigned) time(&t));

	return rand() % max;
}

void high_wait(int cycles){
	long j = 0;
  for(j=0;j<cycles;j++)
		j = j;
}

void high_image(){
	 long i;
	 int green, red, vis;
	 for(i=0; i<80; i++){
	  //RGB turned into an integer value for comparison
	  red = (high_buffer[2*i] & 0xF8);
	  green = (((high_buffer[2*i] & 0x07) << 5) | ((high_buffer[2*i+1] & 0xE0) >> 3));
	  if(green + 20 < red){
			//Green is usually much higher then red due the the extra bit place in RGB565
	   high_numbuffer[i] = 1;
	   vis +=1;
	  }
		else{
	   high_numbuffer[i] = 0;
	  }
	  //If Red is visable then high_isRedVisable turns to true
	  if(vis>0){
	   high_isRedVisable = 1;
	  }else{
	   high_isRedVisable = 0;
	  }
	 }
}

void high_get_image(){
 	e_poxxxx_launch_capture((char *)high_buffer);
	while(!e_poxxxx_is_img_ready()){};
}
