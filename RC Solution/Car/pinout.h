/*
 * pinout.h
 *
 * Created: 24.02.2018 13:43:28
 *  Author: Victor
 */ 


#ifndef PINOUT_H_
#define PINOUT_H_


#define LORA_RESET
#define LORA_NSS PB2
#define LORA_DIO0 PD2
#define LORA_DIO1
#define LORA_DIO2
#define LORA_DIO3
#define LORA_DIO4
#define LORA_DIO5
#define SERVO_ESC BIT(0)
#define SERVO_STEERING BIT(1)
#define SERVO_LIGHTS BIT(2)
#define SERVO_CAM BIT(3)
#define SWITCH_CAM
#define SENSOR_RPM

#define DEBUG_LED PB5


#endif /* PINOUT_H_ */