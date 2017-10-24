/*
 * sx1278_defs.h
 *
 * Created: 24.10.2017 23:08:37
 *  Author: Victor
 */ 


#ifndef SX1278_DEFS_H_
#define SX1278_DEFS_H_
#define MAX_PAYLOAD 0
typedef struct
{
	//! Structure Variable : Packet destination
	/*!
	*/
	uint8_t dst;

	//! Structure Variable : Packet source
	/*!
	*/
	uint8_t src;

	//! Structure Variable : Packet number
	/*!
	*/
	uint8_t packnum;

	//! Structure Variable : Packet length
	/*!
	*/
	uint8_t length;

	//! Structure Variable : Packet payload
	/*!
	*/
	uint8_t data[MAX_PAYLOAD];

	//! Structure Variable : Retry number
	/*!
	*/
	uint8_t retry;
} pack;




#endif /* SX1278_DEFS_H_ */