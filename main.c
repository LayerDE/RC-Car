//Interner Takt 3.3V
#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include "atmega328p_pinout.h"
#include "SX1278_def.h"

typedef void (*task)(void);
char cData;
unsigned char schedule_first;
unsigned char schedule_last;
task scheduler[32];
task ADC_task;//reserved
unsigned char spi_buffer[2][4];
unsigned char* spi_buffer_ptr[];
ISR(ADC_vect)//ADC isr reserved
{
	ADC_task();
}

ISR(INT0_vect)//lora pack reciving
{
	/* interrupt code here */
}

void SPI_MasterInit(void)
		{
			/* Set MOSI and SCK output, all others input */
			DDRB = (1<<DDB3)|(1<<DDB5);
			/* Enable SPI, Master, set clock rate fck/16 */
			SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

		}

// utterly pointless separate 'functions'
#define SPI_write(c)    spi(c)
#define SPI_read(c)     spi(0)

uint8_t spi(uint8_t c)
{
	SPDR = c;
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
void add_schedule(task schedule) {
}

int main(void)
{
	PRR = (0<<PRSPI);
	DDRB = (1<<DDB0)|(1<<DDB3)|(1<<DDB5);
	PORTB = (0<<PB0);
	SPI_MasterInit();
    while(1)
    {	
		SPI_write(cData);
    }
}