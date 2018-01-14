/*
 * RC-Car.c
 *
 * Created: 24.10.2017 22:06:32
 * Author : Victor
 * 
 */ 
// Interner Takt 8Mhz@3.3V
// AVR is little Edian (like x86) will be importat by swiching transmitter controller
#define F_CPU 8000000UL

#define BIT(n) (1<<n)
#define CONFIG_BYTE(byte,on,off) byte = ((byte & (0xFF^(off)))|on)

#include <avr/io.h>			// for Pin IO
#include <avr/interrupt.h>	// for Interrupts
#include <avr/sleep.h>		// for IDLE mode
#include <avr/eeprom.h>		// for programming values
#include <avr/pgmspace.h>	// for constant arrays
#include <avr/wdt.h>
#include "sx1278_defs.h"
#include "RC-CommandsV1.h"


/**
 * Use with A,B,C or D
 */
#define DDR(x) DDR##x
#define PORT(x) PORT##x
#define PIN(x) PIN##x


// task framework
typedef void (*task)(void);//task is my name for a function pointer used in the scheduler
//typedef void (*ADC_task)(uint16_t);//task is my name for a function pointer used in the scheduler
// SCHEDULE_MAX-1 tasks work without any bugs  SCHEDULE_MAX must be 2^n for the performance (if its not 2^n its a performance issue)
#define SCHEDULE_MAX (1<<4)
#if (SCHEDULE_MAX & (SCHEDULE_MAX - 1))
	//slower but usable for n tasks
	#define SCHEDULE_INC(x) ((x + 1) > SCHEDULE_MAX ? (++x) : (x=0))
#else
	//faster but only for 2^n tasks
	#define SCHEDULE_INC(x) (x = (x + 1) & (SCHEDULE_MAX - 1))
#endif 
#define default_schedule sleep

//sleep task for saving energy by idleing the µC
void sleep(){
	set_sleep_mode(SLEEP_MODE_IDLE);//setup IDLE mode. only 1/4 powerconsumption
	sleep_mode();//goto sleep
}


unsigned char schedule_first = 0;
unsigned char schedule_last = SCHEDULE_MAX - 1;
//-std=gnu99
task scheduler[SCHEDULE_MAX]={ [ 0 ... (SCHEDULE_MAX-1) ] = default_schedule };

void add_task(task schedule) {// add task to scheduler
	scheduler[SCHEDULE_INC(schedule_last)]=schedule;
}
// end task framework

//SPI framework
#define SPI_BUFFER_SIZE (1 << 7)
unsigned char spi_buffer_index = 0;
unsigned char spi_buffer[SPI_BUFFER_SIZE];
unsigned char spi_pack_start
unsigned char spi_pack_end
//end SPI framework
void spi_buffer_add(uint8_t value){
	spi_buffer[spi_pack_end++]=value;
}
uint8_t spi_buffer_get(){
	return spi_buffer[spi_pack_start++];
}
unsigned int EEMEM deadzone;

int16_t analog_decode(uint16_t analog_in){ //simple void to read analog input for servo with deadone (will b improved later)
	int16_t tmp=analog_in-(~(uint16_t)0)/2);
	if(tmp > 0) 
		return (tmp - deadzone) < 0 ? 0 : (tmp - deadzone);
	else
		return (tmp + deadzone) > 0 ? 0 : (tmp + deadzone);
}
//tasks
void read_steering(){
	spi_buffer_add(RC_STEERING)
	
}
void read_throttle(){
	spi_buffer_add(RC_THROTTLE)
	spi_buffer_add
}

task ADC_task; // reserved
uint16_t ADC_value;
/*
void get_lora_package(){//reserved later car func
	unsigned char pack_size=SPI_read();
	for(unsigned char i=0;i<HEADERSIZE;i++) spi_buffer[!spi_buffer_index][i]=SPI_read();
	spi_buffer_index=!spi_buffer_index;
	for(unsigned char i=0;i<pack_size;i++) exec_command(SPI_read());
}
*/
#define throw_ERROR add_task(LED_on)
void LED_on(){
	PORT(B)|=(1<<5);
	wdt_reset();//reset interval for error spamming
	WDTCSR |= (1<<WDIE);//start watchdog
}
void LED_off(){
	PORTB&=~(1<<5);
}

// teil der uhr
#define CLK_PRECOUNT (0x0100-200)
unsigned char precount=0;
unsigned int seconds=0;

void clock_inc(){ // kleine ungenaue uhr die mitl�uft
	if(++precount) return;
	precount=CLK_PRECOUNT;
	seconds++;
}

void init_spi_lora()
{
	// Set MOSI and SCK output, all others input
	DDR(B) = (1 << DDB3) | (1 << DDB5);
	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1 << SPE) | (1 << MSTR ) | (1 << SPR0);
	
	//INT0 for pack reciving
	EIMSK |= (1 << INT0);     // Turns on INT0
	
	sei();//enable interrupts
}
void init_watchdog(){//for debug LED or another stupid function
	CONFIG_BYTE(WDTCSR,BIT(WDP2)|BIT(WDIE),BIT(4/*WDTOE*/)|BIT(WDP1)|BIT(WDP0)|BIT(WDE));
	wdt_reset();
}

#define SOFT_RESET init()
void init(){ // first task to execute
	cli();
	for(unsigned char i=0;i<SCHEDULE_MAX;i++) scheduler[i]=default_schedule; // set all tasks to sleep
	schedule_first = 0;
	schedule_last = SCHEDULE_MAX - 1;
	add_task(init_spi_lora);
	add_task(init_watchdog);
}

// end tasks
ISR(ADC_vect)// ADC isr reserved
{
	/*for using the ISR
	ADMUX = (1<<REFS0);               // Set Reference to AVCC and input to ADC0
	ADCSRA = (1<<ADEN)|(1<<ADPS2)     // Enable ADC, set prescaler to 16
	|(1<<ADIE);               // Fadc=Fcpu/prescaler=1000000/16=62.5kHz
	*/
	add_task(ADC_task);
}

ISR(INT0_vect) // lora pack reciving
{
	/*
		low
		EICRA |= ( (0 << ISC01) | (0 << ISC00) );
		//logical change
		EICRA |= ( (0 << ISC01) | (1 << ISC00) );
		//falling edge
		EICRA |= ( (1 << ISC01) | (0 << ISC00) );
		//ENABLE INT0 ON RISING EDGE
		EICRA |= ( (1 << ISC01) | (1 << ISC00) );
		EIMSK |= (1 << INT0); //ENABLE INT0

	}
	*/
	
	//add_task(sleep);
}


ISR(SPI_STC_vect) // spi per interrupt for higher performance
{
	SPDR = spi_buffer[++spi_buffer_index];
	spi_buffer[spi_buffer_index] = SPDR;
	
}

ISR (TIMER1_COMPA_vect)
{
	PORT(C)|=SERVO_PIN[servo_index];
	OCR1B=servo_buffer[servo_index++];
	servo_index&=0x0F;
	// reset timer1
	add_task(clock_inc);
}

ISR (TIMER1_COMPB_vect)
{
	PORT(C)&=0xF0;
	// servocontrol

}

ISR(WDT_vect){
	add_task(LED_off);
}

int main(void)
{
	add_task(init); // run init with scheduler
	main_loop: // while(1){
		// Contexswitch means stack mark and goto not
		scheduler[schedule_first]();
		scheduler[schedule_first]=default_schedule;
		
		// if(schedule_first!=schedule_last)
		//	SCHEDULE_INC(schedule_first);//looks better but doesnt work with SCHEDULE_MAX tasks because the task sticks
		
		(schedule_first==schedule_last)?
			schedule_first=SCHEDULE_INC(schedule_last) : 
			SCHEDULE_INC(schedule_first); // SCHEDULE_MAX tasks works as long as now new task will be added
	goto main_loop;//}
}