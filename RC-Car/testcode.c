//car
void servo_tester(){ // pythonsytyler :D
	if(!(seconds%4))
		for(char x=0;x<SERVO_COUNT;x++)
			servo_buffer[x]=servo_min_ram[x];
	else if(!(seconds%2))
		for(char x=0;x<SERVO_COUNT;x++)
			servo_buffer[x]=servo_min_ram[x];
}
@override
void clock_inc(){ // kleine ungenaue uhr die mitl�uft
	if(++precount) return;
	precount=CLK_PRECOUNT;
	seconds++;
	add_task(servo_tester);
}
@override
unsigned int EEMEM servo_mid_eeprom[SERVO_COUNT]={1500*8/PRESCALER,1500*8/PRESCALER,1500*8/PRESCALER,1500*8/PRESCALER};
unsigned int EEMEM servo_min_eeprom[SERVO_COUNT]={1000*8/PRESCALER,1000*8/PRESCALER,1000*8/PRESCALER,1000*8/PRESCALER};
unsigned int EEMEM servo_max_eeprom[SERVO_COUNT]={2000*8/PRESCALER,2000*8/PRESCALER,2000*8/PRESCALER,2000*8/PRESCALER};
//transmitter