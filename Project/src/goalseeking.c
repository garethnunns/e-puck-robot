
void goalseeking(int selection){

	int prox0;
	int prox6;
	int prox7;
	while(getselector() == selection){
		long i;
		prox0 = e_get_prox(0);
		prox7 = e_get_prox(7);
		prox6 = e_get_prox(6);
		if(prox0 > 300 ||  prox7 > 300 ||  prox6 > 500){
			e_set_led(0,1);
			e_set_speed_left(300);
			e_set_speed_right(-300);
		}
		else{
			e_set_led(0,0);
			e_set_speed_left(300);
			e_set_speed_right(300);
		}
		for(i=0; i<100000; i++) { asm("nop"); }
	}
	
	
}