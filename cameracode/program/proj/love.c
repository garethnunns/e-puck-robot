#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"	
#include "../lib/motor_led/e_led.h"
#include "../lib/a_d/e_prox.h"

int love (int selection)
{
	
	e_init_port();
	e_init_prox();
	e_init_motors();

    love_change(0, 0);   
    int IRS = 0;                //Original Detection sensor
    int IRSN = 0;               //New Detection sensor
    int IRSV = e_get_prox(0);   //Current Detection Sensor value
    int i = 0;

	while(1)
    {
        IRSV = 0;
        for(i=0;i<8;i++)                                  //Loop for sensor reading
        {
            if(e_get_prox(i)>IRSV)                           //Sets Highest detection
            {
                IRSN = i;
                IRSV = e_get_prox(i);
            }
        }
        if(IRSN != IRS)                                  //Calls movement function if highest changes
        {
            IRS = IRSN;
            love_change(IRS, IRSV);
        }
    }
}



void love_change (int IRS, int IRSV)
{
        e_set_led(0, 0);
        e_set_led(1, 0);
        e_set_led(5, 0);
        e_set_led(7, 0);
        if(e_get_prox(0)>=IRSV || e_get_prox(7)>=IRSV)        //Drives forward
        {
           	if(IRSV > 300){
				love_reverse();
			}else if(IRSV > 200){
				love_stop();
				//	love_set_leds();
			}else{
				love_go_straight();
			}
        }
        else
        {   
            if(IRS==1 || IRS==2  || IRS== 3)                  //Turns Right
            {             
                e_set_speed_left(500);
                e_set_speed_right(-500);
                e_set_led(1, 1);
            }
            else                                              //Turns Left
            {
                 if(IRS==4 /* || IRS == 5 */ || IRS==6)		//Broken Sensor
                 {
                      e_set_speed_left(-500);  
                      e_set_speed_right(500);
                      e_set_led(7, 1);
                 }
                 else
                 {
                      e_set_speed_left(500);
                      e_set_speed_right(500);
                      e_set_led(5, 1);
                 }
            }
        }  
}

void love_go_straight(void){
	e_set_speed_left(500);
	e_set_speed_right(500);
}

void love_reverse(void){
	e_set_speed_left(-500);
	e_set_speed_right(-500);
}

void love_stop(void){
	e_set_speed_left(0);
	e_set_speed_right(0);
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