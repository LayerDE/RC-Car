/*
 * sx1278_defs.h
 *
 * Created: 24.10.2017 23:08:37
 *  Author: Victor
 */ 
#ifndef RC_COMMAND_INTERFACE_V1_
#define RC_COMMAND_INTERFACE_V1_
#define _4BIT(NAME,x) #define NAME_0 0x##x 0\
#define NAME_1 0x##x 1\
#define NAME_2 0x##x 2\
#define NAME_3 0x##x 3\
#define NAME_4 0x##x 4\
#define NAME_5 0x##x 5\
#define NAME_6 0x##x 6\
#define NAME_7 0x##x 7\
#define NAME_8 0x##x 8\
#define NAME_9 0x##x 9\
#define NAME_A 0x##x A\
#define NAME_B 0x##x B\
#define NAME_C 0x##x C\
#define NAME_D 0x##x D\
#define NAME_E 0x##x E\
#define NAME_F 0x##x F\

#define RC_STEERING 0xF0
#define RC_THROTTLE 0xE1
#define _4BIT(RC_SET_EEPROM,9)
#define _4BIT(RC_GET_EEPROM,A)
//USED F0 E1 9x Ax
#endif /* RC_COMMAND_INTERFACE_V1_*/