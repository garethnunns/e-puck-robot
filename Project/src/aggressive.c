#include "../lib/motor_led/e_init_port.h"
#include "../lib/motor_led/e_motors.h"	
#include "../lib/motor_led/e_led.h"
#include "../lib/a_d/e_prox.h"

int aggressive (int selection)
{
    aggressive_change(0, 0);   
    int IRS = 0;                //Original Detection sensor
    int IRSN = 0;               //New Detection sensor
    int IRSV = e_get_prox(0);   //Current Detection Sensor value
    int i = 0;

	while(getselector() == selection)
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
            aggressive_change(IRS, IRSV);
        }
    }
}



void aggressive_change (int IRS, int IRSV)
{
        e_set_led(0, 0);
        e_set_led(1, 0);
        e_set_led(5, 0);
        e_set_led(7, 0);
        if(e_get_prox(0)>=IRSV || e_get_prox(7)>=IRSV)        //Drives forward
        {
            e_set_speed_left(1000);
            e_set_speed_right(1000);
            e_set_led(0, 1);
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
                 if(IRS==4 || IRS==6)
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



int aggressive_wait(void)
{
   int j = 0;
   for(j=0;j<10000;j++)
   {
        j=j;
   }
   return 0;
}
