// AVR is little Edian (like x86) will be importat by swiching transmitter controller
#define F_CPU_MHz (F_CPU/1000000UL)
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
#define SPI_EMPTY 0x00
#define SPI_BUFFER_SIZE (1 << 7)
unsigned char spi_buffer_index = 0;
unsigned char spi_buffer[SPI_BUFFER_SIZE];
unsigned char spi_pack_start = 0;
unsigned char spi_in_start = 0;
unsigned char spi_pack_end = 0;
//end SPI framework
void spi_buffer_add(uint8_t value){
	spi_buffer[spi_pack_end++]=value;
}

//tasks
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

void spi_worker(){
	while(spi_pack_end - spi_pack_start > 2){
		exec_command(spi_buffer[spi_pack_start],spi_buffer[spi_pack_start+1]);
		spi_buffer[spi_pack_start] = spi_buffer[spi_pack_start+1] = SPI_EMPTY;
		spi_pack_start += 2;
	}
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
//end tasks

//ISRs
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

ISR(WDT_vect){
	add_task(LED_off);
}
//end ISRs
void init();

int main(void)
{
	reset_scheduler();
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