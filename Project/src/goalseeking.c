#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"
#include "../lib/motor_led/e_led.h"
#include "../lib/uart/e_uart_char.h"
#include "../lib/a_d/e_ad_conv.h"
#include "../lib/a_d/e_prox.h"
#include "../lib/utility.h"

//void advanceForwards(){	
//}
//
//


int frontObstacleCount = 0;
int rightTurnCount = 0;
int leftTurnCount = 0;
long leftSteps = 0;
long rightSteps = 0;
long loopCounter = 0;


void goalseek_wait(long value)
{
   long a;
   for(a=0;a<value;a++)
   {
       	asm("nop");
   }
}

void turnLightsOn(){
	//this is an infinite wait :)
	e_set_speed_left(0);
	e_set_speed_right(0);
	int fl;
	for(fl=0; fl<100000; fl++){
		e_set_front_led(1);
		e_set_body_led(1);
		e_set_front_led(0);
		e_set_body_led(0);
	}
	wait(50000000);
}

int checkIfClear(){
	int prox4;
	int prox5;
	prox5 = e_get_prox(5);
	if(prox5 > 300){
		return 1;
	} else {
		return 0;
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
}

void turnLeftNinetyDegrees(){
	e_set_steps_right(0); // reset right steps
	e_set_led(0,0);
	do{
		e_set_led(4,1);	 	
		e_set_speed_left(-325);
		e_set_speed_right(325);
		rightSteps = e_get_steps_right();
		goalseek_wait(10000);
	}while(rightSteps<=325);
	leftTurnCount++;  		   
}

void obstacleAvoid(){
	turnRightNinetyDegrees();
	turnLightsOn(); 
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
		} else { 
			if(prox0 > 300 ||  prox7 > 300){
				e_set_speed_left(0); // stop moving
				e_set_speed_right(0);
				e_set_led(0,1);
				frontObstacleCount++;  // bump up obstacle count
				//loopCounter++;
			} else {
				e_set_speed_left(300); // move forwards
				e_set_speed_right(300);
				e_set_led(0,0);
				//loopCounter++;
	  		}
		}   		      
		for(i=0; i<100000; i++) { asm("nop"); }
		detectFrontObstacle();   
}


void goalseeking(int selection){
	e_init_motors();
	e_init_prox();
	long j;
	if(getselector() == selection){
		detectFrontObstacle();
	}
	for(j=0; j<100000; j++) { asm("nop"); }	
}

