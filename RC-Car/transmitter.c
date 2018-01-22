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

#include "Universal.h"


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