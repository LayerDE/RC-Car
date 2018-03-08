/*
 * RC-Lib.c
 *
 * Created: 24.02.2018 00:19:30
 * Author : Victor
 */ 

#include <avr/io.h>			// for Pin IO
#include "../Macros.h"
#include <avr/interrupt.h>	// for Interrupts
#include <avr/sleep.h>		// for IDLE mode
#include <avr/eeprom.h>		// for programming values
#include <avr/pgmspace.h>	// for constant arrays
#include <avr/wdt.h>

// task framework
typedef void (*task)(void);//task is my name for a function pointer used in the scheduler
//typedef void (*ADC_task)(uint16_t);//task is my name for a function pointer used in the scheduler
// SCHEDULE_MAX-1 tasks work without any bugs  SCHEDULE_MAX must be 2^n for the performance (if its not 2^n its a performance issue)

//uint8_t eeprom_read_byte (const uint8_t *addr)
//void eeprom_write_byte (uint8_t *addr, uint8_t value)
//uint16_t eeprom_read_word (const uint16_t *addr)
//void eeprom_write_word (uint16_t *addr, uint16_t value)
//void eeprom_read_block (void *pointer_ram, const void *pointer_eeprom, size_t n)
//void eeprom_write_block (void *pointer_eeprom, const void *pointer_ram, size_t n)

//SPI framework
unsigned char spi_buffer_index = 0;
unsigned char spi_buffer[SPI_BUFFER_SIZE];
unsigned char spi_pack_start = 0;
unsigned char spi_in_start = 0;
unsigned char spi_pack_end = 0;
//end SPI framework
void spi_buffer_add(uint8_t value){
	spi_buffer[spi_pack_end++]=value;
}

//sleep task for saving energy by idleing the µC
void sleep(){
	set_sleep_mode(SLEEP_MODE_IDLE);//setup IDLE mode. only 1/4 powerconsumption
	sleep_mode();//goto sleep
}

// teil der uhr

unsigned char precount=0;
unsigned int seconds=0;


void clock_inc(){ // kleine ungenaue uhr die mitläuft
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


unsigned char schedule_first = 0;
unsigned char schedule_last = SCHEDULE_MAX - 1;
//-std=gnu99
task scheduler[SCHEDULE_MAX]={ [ 0 ... (SCHEDULE_MAX-1) ] = default_schedule };

//V2
void add_task(task schedule) {// add task to scheduler
	uint8_t temp = SCHEDULE_MAX;
	while(--temp)
		if(scheduler[SCHEDULE_INC(schedule_last)]==default_schedule)
			break;//fix scheduler for SCHEDULE_MAX task concept
	scheduler[schedule_last]=schedule;
}

void LED_off(){
	PORTB&=~(1<<5);
}

void reset_scheduler(){
	cli();
	for(unsigned char i=0;i<SCHEDULE_MAX;i++) scheduler[i]=default_schedule; // set all tasks to sleep
	schedule_first = 0;
	schedule_last = SCHEDULE_MAX - 1;
	sei();
}
