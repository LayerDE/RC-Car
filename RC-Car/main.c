/*
 * RC-Car.c
 *
 * Created: 24.10.2017 22:06:32
 * Author : Victor
 * 
 */ 
// Interner Takt 8Mhz@3.3V
// AVR is little Edian (like x86)
#define F_CPU 8000000UL

#define BIT(n) (1<<n)
#define CONFIG_BYTE(byte,on,off) byte = ((byte & (0xFF^(off)))|on)

#include <avr/io.h>			// for Pin IO
#include <avr/interrupt.h>	// for Interrupts
#include <avr/sleep.h>		// for IDLE mode
#include <avr/eeprom.h>		// for programming values
#include <avr/pgmspace.h>	// for constant arrays
#include "pinout_car.h"
#include "sx1278_defs.h"

/*
(x can be replaced by A,B,C,D as per the AVR you are using)
– DDRx register
– PORTx register
– PINx register
*/


// task framework
typedef void (*task)(void);

#define schedule_max (1<<4)
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

//SPI framework
#define SPI_BUFFER_SIZE (1 << 5)
unsigned char spi_buffer_index = 0;
unsigned char spi_buffer[SPI_BUFFER_SIZE];
//end SPI framework

// Car specific functions
int servo_buffer[4];
unsigned int eeprom_ptr=0xFFFF;
void exec_command(unsigned int* command){
	if((*command)&1) servo_buffer[(*command)&(0x02)]=(*command)/0x04;
	
}
// end Car specific functions

//tasks
void sleep(){
	set_sleep_mode(SLEEP_MODE_IDLE);//setup IDLE mode. only 1/4 powerconsumption
	sleep_mode();//goto sleep
}

task ADC_task; // reserved

/*
void get_lora_package(){//reserved later car func
	unsigned char pack_size=SPI_read();
	for(unsigned char i=0;i<HEADERSIZE;i++) spi_buffer[!spi_buffer_index][i]=SPI_read();
	spi_buffer_index=!spi_buffer_index;
	for(unsigned char i=0;i<pack_size;i++) exec_command(SPI_read());
}
*/
#define SERVO_COUNT 4
#define PRESCALER 1
#define SERVO_PERIODE (unsigned int)20000/4*8/PRESCALER
#define SERVO_CALC(int16) (SERVO_MIDDLE*8/PRESCALER+int16)
// declare an eeprom array
unsigned char servo_index = 0; // index of the servo who gets signal now
unsigned int EEMEM servo_mid_eeprom[SERVO_COUNT]={1500*8/PRESCALER,1500*8/PRESCALER,1500*8/PRESCALER,1500*8/PRESCALER};
unsigned int EEMEM servo_min_eeprom[SERVO_COUNT]={1000*8/PRESCALER,1000*8/PRESCALER,1000*8/PRESCALER,1000*8/PRESCALER};
unsigned int EEMEM servo_max_eeprom[SERVO_COUNT]={2000*8/PRESCALER,2000*8/PRESCALER,2000*8/PRESCALER,2000*8/PRESCALER};
// declare a ram array
unsigned int servo_mid_ram[SERVO_COUNT];
unsigned int servo_min_ram[SERVO_COUNT];
unsigned int servo_max_ram[SERVO_COUNT];
unsigned const char SERVO_PIN[SERVO_COUNT] PROGMEM={SERVO_ESC,SERVO_LIGHTS,SERVO_STEERING,SERVO_CAM}; 
void init_servos(){
	// direction
	// WGM1 to mode 4 for clear on compare with OCR1A
	CONFIG_BYTE(TCCR1A , BIT(COM1A1) | BIT(COM1B1) , BIT(WGM10) | BIT(WGM11));// FastPWM Mode mode TOP determined by ICR1 - non-inverting Compare Output mode
	CONFIG_BYTE(TCCR1B , BIT(WGM12) | BIT(CS10) , BIT(CS11) | BIT(CS12) | BIT(WGM13));    // set prescaler to 1, FastPWM Mode mode continued
	CONFIG_BYTE(TIFR0 , BIT(OCF1A) | BIT(OCF1B),0);
	
	DDRC = 0x0F; // Servo out

	//ICR1 = 20000;      // set period to 20 ms
	OCR1A = SERVO_PERIODE;      // set count to 1500 us - 90 degree
	OCR1B = servo_mid_eeprom[servo_index];      // set count to 1500 us - 90 degree
	TCNT1 = 0;         // reset timer
}

void init_spi_lora()
{
	// Set MOSI and SCK output, all others input
	DDRB = (1 << DDB3) | (1 << DDB5);
	// Enable SPI, Master, set clock rate fck/16
	SPCR = (1 << SPE) | (1 << MSTR ) | (1 << SPR0);
	
	//INT0 for pack reciving
	EIMSK |= (1 << INT0);     // Turns on INT0
	
	sei();//enable interrupts
}

void init(){ // first task to execute
	for(unsigned char i=0;i<schedule_max;i++) scheduler[i]=default_schedule; // set all tasks to sleep
	add_task(init_spi_lora);
	add_task(init_servos);
}

// teil der uhr
#define CLK_PRECOUNT (0x0100-200)
unsigned char precount=0;
unsigned int seconds=0;
void clock_inc(){ // kleine ungenaue uhr die mitläuft
	if(++precount) return;
	precount=CLK_PRECOUNT;
	seconds++;
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
	
	add_task(sleep);
}


ISR(SPI_STC_vect) // spi per interrupt for higher performance
{
	SPDR = spi_buffer[++spi_buffer_index];
	spi_buffer[spi_buffer_index] = SPDR;
	
}

ISR (TIMER1_COMPA_vect)
{
	PORTC|=
	OCR1B=servo_buffer[servo_index++];
	servo_index&=0x0F;
	// reset timer1
	add_task(clock_inc);
}

ISR (TIMER1_COMPB_vect)
{
	PORTC&=0xF0;
	// servocontrol

}

int main(void)
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