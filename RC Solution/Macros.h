#ifndef _Macros
#define _Macros

#define F_CPU_MHz (F_CPU/1000000UL)
#define BIT(n) (1<<n)
#define CONFIG_BYTE(byte,on,off) byte = ((byte & (0xFF^(off)))|on)

#define SCHEDULE_MAX (1<<4)
#if (SCHEDULE_MAX & (SCHEDULE_MAX - 1))
//slower but usable for n tasks
#define SCHEDULE_INC(x) ((x + 1) > SCHEDULE_MAX ? (++x) : (x=0))
#else
//faster but only for 2^n tasks
#define SCHEDULE_INC(x) (x = (x + 1) & (SCHEDULE_MAX - 1))
#endif
#define default_schedule sleep

#define DDR(x) DDR##x
#define PORT(x) PORT##x
#define PIN(x) PIN##x

#define SPI_EMPTY 0x00
#define SPI_BUFFER_SIZE (1 << 7)

#define CLK_PRECOUNT (0x0100-200)

#define SERVO_COUNT 4
#define PRESCALER 8
#define SERVO_PERIODE (unsigned int)20000/SERVO_COUNT*F_CPU_MHz/PRESCALER
#define SERVO_CALC(servo,int16) (servo_mid_ram[servo]*F_CPU_MHz/PRESCALER+int16)

#endif