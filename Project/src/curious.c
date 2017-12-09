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

#include "curious.h"


char curBuffer[160];
int curNumbuffer[80];
long curIsGreenVisable;
int curStop = 0;
int curNospin = 0;

//custom cam picture load
void curGetImage(){
	e_poxxxx_launch_capture((char *)curBuffer);
    while(!e_poxxxx_is_img_ready()){};
}
// Image processing removes useless information
void curImage(){
	long i;
	int green, red, vis;
	for(i=0; i<80; i++){
		//RGB turned into an integer value for comparison
		red = (curBuffer[2*i] & 0xF8);
		green = (((curBuffer[2*i] & 0x07) << 5) | ((curBuffer[2*i+1] & 0xE0) >> 3));
		if(red > green + 20){ //Green is usually much higher then red due the the extra bit place in RGB565
			curNumbuffer[i] = 1;
			vis +=1;
		}else{
			curNumbuffer[i] = 0;
		}
		//If Green is visable then isGreenVisable turns to true
		if(vis>0){
			curIsGreenVisable = 1;
		}else{
			curIsGreenVisable = 0;
		}
	}
}

//Decide which way to turn based on the number of true pixels.
int curTurnDirection(){
	int sumL = 0;
	int sumR = 0;
	int i;
	for(i=0;i<40;i++){
		sumL += curNumbuffer[i];
		sumR += curNumbuffer[i+40];
	}
	if(sumL<sumR){
		return 1;
	}else{
		return 0;
	}
}
//Function to deal with turning.
void curTurn(void) {
	if(curTurnDirection()){
		e_set_speed_left (500);
		e_set_speed_right(-500);
	}else{
		e_set_speed_left (-500);
		e_set_speed_right(500);
	}
}
void curForward(int speed){
	e_set_speed_left (speed);
	e_set_speed_right(speed);
}

void curAllLED(void){
	e_set_led(0,1);
	e_set_led(1,1);
	e_set_led(2,1);
	e_set_led(3,1);
	e_set_led(4,1);
	e_set_led(5,1);
	e_set_led(6,1);
	e_set_led(7,1);
}

void curCalculateSpeed(void) {

	int counter = 0;
	int x;

	for(x=0;x<80;x++){
		if (curNumbuffer[x] == 1){
			counter += 1;
		}
	}

	if (counter >= 75) {
		curForward(0);
		curAllLED();
		curStop = 1;
		curForward(-1000);
	} else if (counter > 40) {
		curForward(500);
	} else if (counter > 20){
		curForward(750);
	} else
		curForward(1000);
}

//Main function of follower
void curious(void){



	e_init_port();
	e_init_uart1();
	e_init_motors();
	//basic set up for the camera and
	e_poxxxx_init_cam();
	e_poxxxx_config_cam(0,(ARRAY_HEIGHT - 4)/2,640,4,8,4,RGB_565_MODE);
	e_poxxxx_write_cam_registers();

	e_start_agendas_processing();
	int centreValue;

	//while loop with ir cutout

	//move below into function
	while(curStop == 0){
		curGetImage();
		curImage();
		e_led_clear();

		//Take a section of the center, this means if there is an error with one it won't effect it as a whole.
		centreValue = curNumbuffer[38] + curNumbuffer[39] + curNumbuffer[40] + curNumbuffer[41] + curNumbuffer[42] + curNumbuffer[43]; // removes stray
		if(centreValue > 3){ //If green is in the middle then it will go forward
			e_destroy_agenda(curTurn);
			//forward();

			curCalculateSpeed();
			curNospin = 1;
			e_set_led(0,1);
		} else if(curNospin == 0){// if green isn't visible and no true values it will turn left
			e_destroy_agenda(curTurn);
			e_set_speed_left (-500);
			e_set_speed_right(500);
			e_set_led(2,1);
		}
	}
}
