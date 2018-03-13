/*
 * Car.c
 *
 * Created: 24.02.2018 00:17:59
 * Author : Victor
 */ 

#include <avr/io.h>
#include "../Macros.h"
#include "../RC-Command-Interface_V1.h"
#include "../eeprom_adr.h"
	
#include "pinout.h"

int servo_buffer[4];
unsigned int eeprom_ptr=0xFFFF;

ISR(ADC_vect)// ADC isr reserved
{
	/*for using the ISR
	ADMUX = (1<<REFS0);               // Set Reference to AVCC and input to ADC0
	ADCSRA = (1<<ADEN)|(1<<ADPS2)     // Enable ADC, set prescaler to 16
	|(1<<ADIE);               // Fadc=Fcpu/prescaler=1000000/16=62.5kHz
	*/
	add_task(ADC_task);
}
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

ISR(ADC_vect)// ADC isr reserved
{
	/*for using the ISR
	ADMUX = (1<<REFS0);               // Set Reference to AVCC and input to ADC0
	ADCSRA = (1<<ADEN)|(1<<ADPS2)     // Enable ADC, set prescaler to 16
	|(1<<ADIE);               // Fadc=Fcpu/prescaler=1000000/16=62.5kHz
	*/
	add_task(ADC_task);
}

unsigned char servo_index = 0; // index of the servo who gets signal now

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
	eeprom_read_block(servo_min_ram,EEPROM_CAR_SERVO_MIN,EEPROM_SERVO_SIZE);
	eeprom_read_block(servo_mid_ram,EEPROM_CAR_SERVO_MID,EEPROM_SERVO_SIZE)
	eeprom_read_block(servo_max_ram,EEPROM_CAR_SERVO_MAX,EEPROM_SERVO_SIZE)
	for(uint8_t x = 0; x<SERVO_COUNT;x++)
		servo_buffer[x]=servo_mid_ram[x];
	// direction
	// WGM1 to mode 4 for clear on compare with OCR1A
	CONFIG_BYTE(TCCR1A , 0 , BIT(WGM10) | BIT(WGM11) | BIT(COM1A1) | BIT(COM1B1));// CTC Mode mode TOP determined by OCR1A //
	CONFIG_BYTE(TCCR1B , BIT(WGM12) | BIT(CS11) , BIT(CS10) | BIT(CS12) | BIT(WGM13));    // set prescaler to 1/8
	CONFIG_BYTE(TIFR1 , BIT(OCF1A) | BIT(OCF1B),0);
	CONFIG_BYTE(TIMSK1 , BIT(OCIE1A)|BIT(OCIE1B), 0);
	DDR(C) = 0x0F; // Servo out

	//ICR1 = 20000;      // set period to 20 ms
	OCR1A = SERVO_PERIODE;      // set count to 1500 us - 90 degree
	OCR1B = servo_buffer[servo_index];      // set count to 1500 us - 90 degree
	TCNT1 = 0;         // reset timer
	sei();
}

void init(){ // first task to execute
	add_task(init_spi_lora);
	add_task(init_servos);
	add_task(init_watchdog);
}

int main(void)//!save function!
{
	reset_scheduler();
	add_task(init); // run init with scheduler
	main_loop: // while(1){
		// Contexswitch means stack mark and goto not
		scheduler[schedule_first]();
		scheduler[schedule_first]=default_schedule;
	
		(schedule_first==schedule_last)?
			//isr comes here schedule last one too high! task will be skipped! and worked after SCHEDULE_MAX tasks!
			//fixed with add_schedule V2
			schedule_last=SCHEDULE_INC(schedule_first) :
			SCHEDULE_INC(schedule_first); // SCHEDULE_MAX tasks works as long as now new task will be added
	goto main_loop;//}
}

