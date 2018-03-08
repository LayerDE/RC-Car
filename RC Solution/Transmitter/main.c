/*
 * Transmitter.c
 *
 * Created: 24.02.2018 00:18:34
 * Author : Victor
 */ 

#include <avr/io.h>
#include "../Macros.h"
#include "../RC-Command-Interface_V1.h"
#include "../universal-Code.h"
#include "pinout.h"


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


void exec_command(unsigned char command0,unsigned char command1){
	switch(command0>>4){
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
	
}

void spi_worker(){
	while(spi_pack_end - spi_pack_start > 2){
		exec_command(spi_buffer[spi_pack_start],spi_buffer[spi_pack_start+1]);
		spi_buffer[spi_pack_start] = spi_buffer[spi_pack_start+1] = SPI_EMPTY;
		spi_pack_start += 2;
	}
}

ISR(SPI_STC_vect) // spi per interrupt for higher performance
{
	SPDR = spi_buffer[++spi_buffer_index];
	spi_buffer[spi_buffer_index] = SPDR;
	
}

ISR(WDT_vect){
	add_task(LED_off);
}

task ADC_task; // reserved
uint16_t ADC_value;


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

void init(){ // first task to execute
	add_task(init_spi_lora);
	add_task(init_servos);
	add_task(init_watchdog);
}


int main(void)
{
	reset_scheduler();
	add_task(init); // run init with scheduler
	main_loop: // while(1){
		// contexswitch means stack mark and goto not
		scheduler[schedule_first]();
		scheduler[schedule_first]=default_schedule;
		
		(schedule_first==schedule_last)?
			schedule_last=SCHEDULE_INC(schedule_first) :
			SCHEDULE_INC(schedule_first); // SCHEDULE_MAX tasks works as long as now new task will be added
	goto main_loop;//}
}

