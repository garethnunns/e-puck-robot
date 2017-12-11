#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/uart/e_uart_char.h"
#include "../lib/a_d/e_ad_conv.h"
#include "../lib/a_d/e_prox.h"
#include "../lib/utility.h"

int frontObstacleCount = 0;
int rightTurnCount = 0;
int leftTurnCount = 0;
int wallClearedCheck = 0;
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


void turnRightNinetyDegrees(){
	e_set_steps_left(0); // reset left steps
	e_set_led(0,0);
	do{
		e_set_led(4,1);	 	
		e_set_speed_left(325);
		e_set_speed_right(-325);
		leftSteps = e_get_steps_left();
		goalseek_wait(10000);
	}while(leftSteps<=325);
	rightTurnCount++;
	e_set_led(4,0);  		   
}

void turnLeftNinetyDegrees(){
	e_set_steps_right(0); // reset right steps
	e_set_led(0,0);
	do{
		e_set_led(4,1);	 	
		e_set_speed_right(325);
		e_set_speed_left(-325);
		rightSteps = e_get_steps_right();
		goalseek_wait(10000);
	}while(rightSteps<=325);
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
		e_set_speed_left(250);
		e_set_speed_right(250);
		returningStepsDone = e_get_steps_left();
		goalseek_wait(10000);
	}	
}

int clearSide(){
	int prox5;
	if(sideCalledCount == 0){
		e_set_speed_left(250);
		e_set_speed_right(250);
		goalseek_wait(2000000);
		sideCalledCount = 1;
	}
	prox5 = e_get_prox(5);
	if(prox5 > 400){	
		e_set_led(3,1);
		e_set_speed_left(250);
		e_set_speed_right(250);
		prox5 = e_get_prox(5);
		goalseek_wait(10000);
		return 0;
	} else {
		goalseek_wait(1500000);
		e_set_led(3,0);
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
		e_set_speed_left(250);
		e_set_speed_right(250);
		prox5 = e_get_prox(5);	
		goalseek_wait(10000);
		return 0;
	}
	else{
		goalseek_wait(1500000);
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
		if(wallCleared == 1){
			if(leftTurnCount == 0){
				turnLeftNinetyDegrees();
			}
			sideCleared = clearSide();
			if(sideCleared == 1){
				turnLeftNinetyDegrees();
				returnToLine();
				turnRightNinetyDegrees();	
			} else {
				obstacleAvoid();
			}
		} else {
			obstacleAvoid();
		}	
	}		
}

void detectFrontObstacle(){
		long i;
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
				e_set_speed_left(0); // stop moving
				e_set_speed_right(0);
				e_set_led(0,1);
				frontObstacleCount++;  // bump up obstacle count
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
		for(j=0; j<100000; j++) { asm("nop"); }
	}
}

