#define EEPROM_SERVO_SIZE (sizeof(unsigned int)*SERVO_COUNT)
#define EEPROM_CAR_SERVO_MID (EEPROM_PAIR_KEY+8) 
#define EEPROM_CAR_SERVO_MAX (EEPROM_CAR_SERVO_MID+EEPROM_SERVO_SIZE)
#define EEPROM_CAR_SERVO_MIN (EEPROM_CAR_SERVO_MAX+EEPROM_SERVO_SIZE)
#define EEPROM_PAIR_KEY 0
#define EEPROM_NAME (EEPROM_CAR_SERVO_MIN+EEPROM_SERVO_SIZE)
#define EEPROM_NAME_LEN 32