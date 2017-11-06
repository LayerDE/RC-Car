/*
 * IC RGB-LED.c
 *
 * Created: 05.11.2017 01:07:14
 * Author : Victor
 */ 

#include <avr/io.h>			// for Pin IO
#include <avr/interrupt.h>	// for Interrupts
#include <avr/sleep.h>		// for IDLE mode
#include <avr/eeprom.h>		// for programming values
#include <avr/pgmspace.h>	// for constant arrays

// task framework
typedef void (*task)(void);

#define schedule_max (1<<3)
// schedule_max-1 tasks work without any bugs  schedule_max must be 2^n for the performance (if its not 2^n its a performance issue)
#define schedule_inc(x) (x = (x + 1) & (schedule_max - 1))
// #define schedule_inc(x) ((x + 1) > schedule_max ? (++x) : (x=0))
#define default_schedule sleep

unsigned char schedule_first = 0;
unsigned char schedule_last = schedule_max - 1;
task scheduler[schedule_max];

void add_task(task schedule) {// add task to scheduler
	scheduler[schedule_inc(schedule_last)]=schedule;
}
// end task framework

// eeprom usage
unsigned char EEMEM persist[64];
unsigned char read_eeprom_8(unsigned char address){
	return persist[address&0x3F];
}
unsigned int read_eeprom_16(unsigned char address){
	return *((unsigned int*)&persist[address&0x3F]);
}
void write_eeprom_8(unsigned char address,unsigned char data){
	persist[address&0x3F]=data;
}
void write_eeprom_16(unsigned char address,unsigned int data){
	*((unsigned int*)&persist[address&0x3F])=data;
}
//end eeprom usage
//tasks
void sleep(){
	set_sleep_mode(SLEEP_MODE_IDLE);//setup IDLE mode. only 1/4 powerconsumption
	sleep_mode();//goto sleep
}

// teil der uhr
#define CLK_PRECOUNT (0x0100-200)
unsigned char precount=0;

void clock_inc(){ // kleine ungenaue uhr die mitläuft
	if(++precount) return;
	precount=CLK_PRECOUNT;

}

void init_timer()
{
	// Clear interrupts, just to make sure
	cli();
	// Set up Port B pin 3 mode to output by setting the proper bit
	// in the DataDirectionRegisterB
	DDRB = 1<<DDB3;

	// set prescale timer to 1/1024th
	TCCR0B |= (1<<CS02) | (1<<CS00);// set CS02 and CS00 bit in TCCR0B

	// enable timer overflow interrupt
	TIMSK0 |=1<<TOIE0;// left shift 1 to TOIE0 and OR with TIMSK
	//  = set TOIE0 bit
	sei(); //start timer
}

void init(){ // first task to execute
	for(unsigned char i=0;i<schedule_max;i++) scheduler[i]=default_schedule; // set all tasks to sleep
	add_task(init_timer);
}
//Timer 0 ~8bit
ISR(TIMER0_OVF_vect) {
	add_task(clock_inc);
}
ISR(TIMER0_COMPA_vect) {

}
ISR(TIMER0_COMPB_vect) {

}
ISR(PCINT0_vect) {

}

ISR(INT0_vect) {

}
ISR(WDT_vect) {//watchdog timeout

}
int main(void)//ISR reset
{
	add_task(init); // run init with scheduler
	main_loop: // While(1){
		// Contexswitch means stack mark and goto not
		scheduler[schedule_first]();
		scheduler[schedule_first]=default_schedule;
		
		// if(schedule_first!=schedule_last)
		//	schedule_inc(schedule_first);//looks better but doesnt work with schedule_max tasks because the task sticks
		
		(schedule_first==schedule_last)?
		schedule_first=schedule_inc(schedule_last) :
		schedule_inc(schedule_first); // schedule_max tasks works as long as now new task will be added
	goto main_loop;//}
}
