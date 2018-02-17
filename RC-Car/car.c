/*
 * RC-Car.c
 *
 * Created: 24.10.2017 22:06:32
 * Author : Victor
 * 
 */ 
// Interner Takt 8Mhz@3.3V
#define F_CPU 8000000UL
#include "Universal.h"
#include "pinout_car.h"


// Car specific functions
int servo_buffer[4];
unsigned int eeprom_ptr=0xFFFF;
uint16_t command_puffer;
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
	if((command0)&1)
		servo_buffer[(command0)&(0x02)]=(command0)/0x04*0x10;
	
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
	int16_t tmp=analog_in-((~(uint16_t)0)/2);
	if(tmp > 0) 
		return (tmp - deadzone) < 0 ? 0 : (tmp - deadzone);
	else
		return (tmp + deadzone) > 0 ? 0 : (tmp + deadzone);
}
//tasks

task ADC_task; // reserved
uint16_t ADC_value;

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