#include "./motor_led/e_epuck_ports.h"

void wait(long num) {
	long a;
   for(a=0;a<num;a++)
   {
       	asm("nop");
   }
}

int getselector() {
	return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}
