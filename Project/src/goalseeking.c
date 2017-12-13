#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/uart/e_uart_char.h"
#include "../lib/a_d/e_ad_conv.h"
#include "../lib/a_d/e_prox.h"
#include "../lib/utility.h"
#include "../lib/camera/fast_2_timer/e_poxxxx.h"
#include "../lib/motor_led/advance_one_timer/e_led.h"

#include "stdio.h"
#include "string.h"
#include "stdlib.h"

char goalseek_buffer[160];
int goalseek_numbuffer[80];
long goalseek_isGreenVisable;
//int stop = 0;
//int nospin = 0;
int frontObstacleCount = 0;
int rightTurnCount = 0;
int leftTurnCount = 0;
int wallClearedCheck = 0;
int sideClearedCheck = 0;
int wallCleared = 0;
int sideCleared = 0;
int wallCalledCount = 0;
int sideCalledCount = 0;
long leftSteps = 0;
long rightSteps = 0;
long returningStepsDone = 0;
long returningSteps = 0;


void goalseek_wait(long value)
{
   long a;
   for(a=0;a<value;a++)
   {
       	asm("nop");
   }


void turnLightsOn(){
	// should be an infinite wait that turns lights on, called when goal is found
	e_set_speed_left(0);
	e_set_speed_right(0);
	e_set_led(8,1);
	goalseek_wait(10000);
	while(1){
		asm("nop");
		goalseek_wait(10000);
	}
}

void checkGreenObstacle(){
	
    e_poxxxx_init_cam();
	e_poxxxx_config_cam(0,(ARRAY_HEIGHT - 4)/2,640,4,8,4,RGB_565_MODE);
	e_poxxxx_write_cam_registers(); 

	e_start_agendas_processing();
	int centreValue;

	goalseek_getImage();
	goalseek_Image();

	centreValue = goalseek_numbuffer[38] + goalseek_numbuffer[39] + goalseek_numbuffer[40] + goalseek_numbuffer[41] + goalseek_numbuffer[42] + goalseek_numbuffer[43]; // removes stray 
	if(centreValue > 3){ //Activates camera and checks if detecting green in the middle  
}
		while(1){
			turnLightsOn();
		}
	}
}

void goalseek_Image(){	
	long i;
	int green, red, vis;
	for(i=0; i<80; i++){	
		red = (goalseek_buffer[2*i] & 0xF8);
		green = (((goalseek_buffer[2*i] & 0x07) << 5) | ((goalseek_buffer[2*i+1] & 0xE0) >> 3));
		if(green > red + 20){ //Checks if green is more than red, accounting for increased number of green sensors
			goalseek_numbuffer[i] = 1;
			vis +=1;
		}else{
			goalseek_numbuffer[i] = 0;
		}
		//Variable becomes true if visible
		if(vis>0){
			goalseek_isGreenVisable = 1;
		}else{
			goalseek_isGreenVisable = 0;
		}
	}	
}

void goalseek_getImage(){
	e_poxxxx_launch_capture((char *)goalseek_buffer);
    while(!e_poxxxx_is_img_ready()){};
}


void turnRightNinetyDegrees(){
	e_set_steps_left(0); // reset left steps
	e_set_led(0,0);
	do{
		e_set_led(4,1);	 	
		e_set_speed_left(315);
		e_set_speed_right(-315);
		leftSteps = e_get_steps_left();
		goalseek_wait(10000);
	}while(leftSteps<=315);
	rightTurnCount++;
	e_set_led(4,0);  		   
}

void turnLeftNinetyDegrees(){
	e_set_steps_right(0); // reset right steps
	e_set_led(0,0);
	do{
		e_set_led(4,1);	 	
		e_set_speed_right(318);
		e_set_speed_left(-318);
		rightSteps = e_get_steps_right();
		goalseek_wait(10000);
	}while(rightSteps<=318);
	leftTurnCount++;
	e_set_led(4,0);  		   
}

void returnToLine(){
	e_set_speed_left(0);
	e_set_speed_right(0);
	e_set_steps_left(0);
	e_set_steps_right(0);
	returningStepsDone = e_get_steps_left();
	while(returningStepsDone < returningSteps){
		e_set_speed_left(500);
		e_set_speed_right(500);
		returningStepsDone = e_get_steps_left();
		goalseek_wait(10000);
	}	
}

int clearSide(){
	int prox5;
	if(sideCalledCount == 0){
		e_set_speed_left(500);
		e_set_speed_right(500);
		goalseek_wait(1000000);
		sideCalledCount = 1;
	}
	prox5 = e_get_prox(5);
	if(prox5 > 500){	
		e_set_led(3,1);
		e_set_speed_left(500);
		e_set_speed_right(500);
		prox5 = e_get_prox(5);
		goalseek_wait(10000);
		return 0;
	} else {
		goalseek_wait(700000);
		e_set_led(3,0);
		sideClearedCheck = 1;
		return 1;
	}
}

int clearWall(){
	int prox5;
	prox5 = e_get_prox(5);
	if(wallCalledCount == 0){
		e_set_steps_left(0);
		e_set_steps_right(0); 
		wallCalledCount = 1;
	}
	if(prox5 > 900){	
		e_set_led(2,1);
		e_set_speed_left(500);
		e_set_speed_right(500);
		prox5 = e_get_prox(5);	
		goalseek_wait(10000);
		return 0;
	}
	else{
		goalseek_wait(800000);
		e_set_led(2,0);
		wallClearedCheck = 1;
		returningSteps = e_get_steps_left();
		return 1;
	}
}


void obstacleAvoid(){
	if(rightTurnCount < 1){
		turnRightNinetyDegrees();
	}
	if(wallClearedCheck == 0){
		wallCleared = clearWall();
		obstacleAvoid();
	} else {
		if(leftTurnCount == 0){
			turnLeftNinetyDegrees();
		}
		if(sideClearedCheck == 0){
			sideCleared = clearSide();
			obstacleAvoid();
		} else {
			e_set_led(1,1);
			turnLeftNinetyDegrees();
			returnToLine();
			turnRightNinetyDegrees();	
		}
	}	
}		

void detectFrontObstacle(){
		int prox0;
		int prox7;
		prox0 = e_get_prox(0);
		prox7 = e_get_prox(7);
		// if obstacle detected by front proximity sensor
		if(frontObstacleCount > 0){
			obstacleAvoid();
			//turnLightsOn();
			// once obstacle has been avoided reset all variables
			frontObstacleCount = 0;
			rightTurnCount = 0;
			leftTurnCount = 0;
			wallClearedCheck = 0;
			sideClearedCheck = 0;
			wallCleared = 0;
			sideCleared = 0;
			wallCalledCount = 0;
			sideCalledCount = 0;
			leftSteps = 0;
			rightSteps = 0;
			returningStepsDone = 0;
			returningSteps = 0;				
		} else { 
			if(prox0 > 1500 ||  prox7 > 1500){
				e_set_speed_left(0); // moving
				e_set_speed_right(0);
				e_set_led(0,1);
				frontObstacleCount++;  // bump up obstacle count
				checkGreenObstacle();
			} else {
				e_set_speed_left(300); // move forwards
				e_set_speed_right(300);
				e_set_led(0,0);
	  		}
		}   		      
}


void goalseeking(int selection){
	e_init_motors();
	e_init_prox();
	long j;
	while(1){
		detectFrontObstacle();
		for(j=0; j<10000; j++) { asm("nop"); }
	}
}

