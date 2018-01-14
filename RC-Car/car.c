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
#define F_CPU_MHz (F_CPU/1000000UL)
#define BIT(n) (1<<n)
#define CONFIG_BYTE(byte,on,off) byte = ((byte & (0xFF^(off)))|on)

#include <avr/io.h>			// for Pin IO
#include <avr/interrupt.h>	// for Interrupts
#include <avr/sleep.h>		// for IDLE mode
#include <avr/eeprom.h>		// for programming values
#include <avr/pgmspace.h>	// for constant arrays
#include <avr/wdt.h>
#include "pinout_car.h"
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
// Car specific functions
int servo_buffer[4];
unsigned int eeprom_ptr=0xFFFF;
uint16_t command_puffer;
void exec_command(unsigned int* command){
	switch(*command>>12){
		case 0x0:
			break;
		case 0x1:
			break;
		case 0x2:
			break;
		case 0x3:
			break;
		case 0x4:
			break;
		case 0x5:
			break;
		case 0x6:
			break;
		case 0x7:
			break;
		case 0x8:
			break;
		case 0x9:
			break;
		case 0xA:
			break;
		case 0xB:
			break;
		case 0xC:
			break;
		case 0xD:
			break;
		case 0xE:
			break;
		case 0xF:
			break;
	}
	if((*command)&1) servo_buffer[(*command)&(0x02)]=(*command)/0x04;
	
}
void _exec_servo(){

}
void _exec_write_eeprom(){

}
void _exec_reload_servo(){
	add_task(init_servos);
}

// end Car specific functions
unsigned int EEMEM deadzone;

int16_t analog_decode(uint16_t analog_in){ //simple void to read analog input for servo with deadone (will b improved later)
	int16_t tmp=analog_in-(~(uint16_t)0)/2);
	if(tmp > 0) 
		return (tmp - deadzone) < 0 ? 0 : (tmp - deadzone);
	else
		return (tmp + deadzone) > 0 ? 0 : (tmp + deadzone);
}
//tasks

task ADC_task; // reserved
uint16_t ADC_value;

#define throw_ERROR add_task(LED_on)
void LED_on(){
	PORT(B)|=(1<<5);
	wdt_reset();//reset interval for error spamming
	WDTCSR |= (1<<WDIE);//start watchdog
}
void LED_off(){
	PORTB&=~(1<<5);
}

#define SERVO_COUNT 4
#define PRESCALER 1
#define SERVO_PERIODE (unsigned int)20000/SERVO_COUNT*F_CPU_MHz/PRESCALER
#define SERVO_CALC(servo,int16) (servo_mid_ram[servo]*F_CPU_MHz/PRESCALER+int16)
// declare an eeprom array
unsigned char servo_index = 0; // index of the servo who gets signal now
unsigned int EEMEM servo_mid_eeprom[SERVO_COUNT];
unsigned int EEMEM servo_min_eeprom[SERVO_COUNT];
unsigned int EEMEM servo_max_eeprom[SERVO_COUNT];
//unsigned int EEMEM servo_antidead_eeprom[SERVO_COUNT];
// declare a ram array
unsigned int servo_mid_ram[SERVO_COUNT];
unsigned int servo_min_ram[SERVO_COUNT];
unsigned int servo_max_ram[SERVO_COUNT];
unsigned const char SERVO_PIN[SERVO_COUNT] PROGMEM={SERVO_ESC,SERVO_LIGHTS,SERVO_STEERING,SERVO_CAM};


uint16_t map_servo(char servo, int16_t in_val){
	//long map(long x, long in_min, long in_max, long out_min, long out_max)
		//return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	return (int32_t)(in_val + 2048) 
		* (int32_t)(servo_max_ram[servo] - servo_min_ram[servo]) 
		/ (int32_t)(2047 + 2048)
			+ servo_min_ram[servo];
}


void init_servos(){
	// direction
	// WGM1 to mode 4 for clear on compare with OCR1A
	CONFIG_BYTE(TCCR1A , BIT(COM1A1) | BIT(COM1B1) , BIT(WGM10) | BIT(WGM11));// FastPWM Mode mode TOP determined by ICR1 - non-inverting Compare Output mode
	CONFIG_BYTE(TCCR1B , BIT(WGM12) | BIT(CS10) , BIT(CS11) | BIT(CS12) | BIT(WGM13));    // set prescaler to 1, FastPWM Mode mode continued
	CONFIG_BYTE(TIFR0 , BIT(OCF1A) | BIT(OCF1B),0);
	
	DDR(C) = 0x0F; // Servo out

	//ICR1 = 20000;      // set period to 20 ms
	OCR1A = SERVO_PERIODE;      // set count to 1500 us - 90 degree
	OCR1B = servo_mid_eeprom[servo_index];      // set count to 1500 us - 90 degree
	TCNT1 = 0;         // reset timer
	sei();
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
	add_task(init_spi_lora);
	add_task(init_servos);
	add_task(servo_tester);
	add_task(init_watchdog);
}
void reset_scheduler(){
	cli();
	for(unsigned char i=0;i<SCHEDULE_MAX;i++) scheduler[i]=default_schedule; // set all tasks to sleep
	schedule_first = 0;
	schedule_last = SCHEDULE_MAX - 1;
	sei();
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