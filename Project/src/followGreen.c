#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_epuck_ports.h"
#include "../lib/motor_led/advance_one_timer/e_motors.h"
#include "../lib/motor_led/advance_one_timer/e_agenda.h"
#include "../lib/uart/e_uart_char.h"
#include "../lib/camera/fast_2_timer/e_poxxxx.h"
#include "../lib/motor_led/advance_one_timer/e_led.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

char buffer[160];
int numbuffer[80];
long isGreenVisable;
int stop = 0;
int nospin = 0; 

//custom cam picture load
void getImage(){
	e_poxxxx_launch_capture((char *)buffer);
    while(!e_poxxxx_is_img_ready()){};
}
// Image processing removes useless information
void Image(){	
	long i;
	int green, red, vis;
	for(i=0; i<80; i++){
		//RGB turned into an integer value for comparison
		red = (buffer[2*i] & 0xF8);
		green = (((buffer[2*i] & 0x07) << 5) | ((buffer[2*i+1] & 0xE0) >> 3));
		if(green > red + 20){ //Green is usually much higher then red due the the extra bit place in RGB565
			numbuffer[i] = 1;
			vis +=1;
		}else{
			numbuffer[i] = 0;
		}
		//If Green is visable then isGreenVisable turns to true
		if(vis>0){
			isGreenVisable = 1;
		}else{
			isGreenVisable = 0;
		}
	}	
}

//Decide which way to turn based on the number of true pixels.
int turnDirection(){
	int sumL = 0;
	int sumR = 0;
	int i;
	for(i=0;i<40;i++){
		sumL += numbuffer[i];
		sumR += numbuffer[i+40];
	}
	if(sumL<sumR){ 
		return 1;
	}else{
		return 0;
	}
}
//Function to deal with turning.
void turn(void) {
	if(turnDirection()){
		e_set_speed_left (500);
		e_set_speed_right(-500);
	}else{
		e_set_speed_left (-500);
		e_set_speed_right(500);
	}
}
void forward(int speed){
	e_set_speed_left (speed);
	e_set_speed_right(speed);
}

void allLED(void){
	e_set_led(0,1);
	e_set_led(1,1);
	e_set_led(2,1);
	e_set_led(3,1);
	e_set_led(4,1);
	e_set_led(5,1);
	e_set_led(6,1);
	e_set_led(7,1);
}

void calculateSpeed(void) {
	
	int counter = 0;
	int x;

	for(x=0;x<80;x++){
		if (numbuffer[x] == 1){
			counter += 1;
		}
	}

	if (counter >= 75) {
		forward(0);
		allLED();
		stop = 1;
	} else if (counter > 40) {
		forward(500);
	} else if (counter > 20){
		forward(750);
	} else
		forward(1000);
}

//Main function of follower
void followGreen(void){
	//basic set up for the camera and 
	e_poxxxx_init_cam();
	e_poxxxx_config_cam(0,(ARRAY_HEIGHT - 4)/2,640,4,8,4,RGB_565_MODE);
	e_poxxxx_write_cam_registers(); 

	e_start_agendas_processing();
	int centreValue;

	//while loop with ir cutout

	//move below into function
	while(stop == 0){
		getImage();
		Image();
		e_led_clear();

		//Take a section of the center, this means if there is an error with one it won't effect it as a whole.
		centreValue = numbuffer[38] + numbuffer[39] + numbuffer[40] + numbuffer[41] + numbuffer[42] + numbuffer[43]; // removes stray 
		if(centreValue > 3){ //If green is in the middle then it will go forward 
			e_destroy_agenda(turn);
			//forward();

			calculateSpeed();
			nospin = 1;
			e_set_led(0,1);
		} else if(nospin == 0){// if green isn't visible and no true values it will turn left
			e_destroy_agenda(turn);
			e_set_speed_left (-500);
			e_set_speed_right(500);
			e_set_led(2,1);
		}
	}
}

