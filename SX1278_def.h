#ifndef SX1278_def
#define SX1278_def

//! REGISTERS //
//							FSK	Commun	LORA
#define        REG_FIFO        					0x00
#define        REG_OP_MODE        				0x01
#define        REG_BITRATE_MSB    			0x02
#define        REG_BITRATE_LSB    			0x03
#define        REG_FDEV_MSB   				0x04
#define        REG_FDEV_LSB    				0x05
#define        REG_FRF_MSB    					0x06
#define        REG_FRF_MID    					0x07
#define        REG_FRF_LSB    					0x08
#define        REG_PA_CONFIG    				0x09
#define        REG_PA_RAMP    					0x0A
#define        REG_OCP    					0x0B
#define        REG_LNA    					0x0C
#define        REG_RX_CONFIG    			0x0D
#define        REG_FIFO_ADDR_PTR  					0x0D
#define        REG_RSSI_CONFIG   			0x0E
#define        REG_FIFO_TX_BASE_ADDR 		    			0x0E
#define        REG_RSSI_COLLISION    			0x0F
#define        REG_FIFO_RX_BASE_ADDR   					0x0F
#define        REG_RSSI_THRESH    			0x10
#define        REG_FIFO_RX_CURRENT_ADDR   				0x10
#define        REG_RSSI_VALUE_FSK	    		0x11
#define        REG_IRQ_FLAGS_MASK    					0x11
#define        REG_RX_BW		    		0x12
#define        REG_IRQ_FLAGS	    					0x12
#define        REG_AFC_BW		    		0x13
#define        REG_RX_NB_BYTES	    					0x13
#define        REG_OOK_PEAK	    			0x14
#define        REG_RX_HEADER_CNT_VALUE_MSB  				0x14
#define        REG_OOK_FIX	    			0x15
#define        REG_RX_HEADER_CNT_VALUE_LSB  				0x15
#define        REG_OOK_AVG	 			0x16
#define        REG_RX_PACKET_CNT_VALUE_MSB  				0x16
#define        REG_RX_PACKET_CNT_VALUE_LSB  				0x17
#define        REG_MODEM_STAT	  					0x18
#define        REG_PKT_SNR_VALUE	  				0x19
#define        REG_AFC_FEI	  			0x1A
#define        REG_PKT_RSSI_VALUE	  				0x1A
#define        REG_AFC_MSB	  			0x1B
#define        REG_RSSI_VALUE_LORA	  				0x1B
#define        REG_AFC_LSB	  			0x1C
#define        REG_HOP_CHANNEL	  					0x1C
#define        REG_FEI_MSB	  			0x1D
#define        REG_MODEM_CONFIG1	 		 		0x1D
#define        REG_FEI_LSB	  			0x1E
#define        REG_MODEM_CONFIG2	  				0x1E
#define        REG_PREAMBLE_DETECT  			0x1F
#define        REG_SYMB_TIMEOUT_LSB  					0x1F
#define        REG_RX_TIMEOUT1	  			0x20
#define        REG_PREAMBLE_MSB_LORA  					0x20
#define        REG_RX_TIMEOUT2	  			0x21
#define        REG_PREAMBLE_LSB_LORA  					0x21
#define        REG_RX_TIMEOUT3	 			0x22
#define        REG_PAYLOAD_LENGTH_LORA			 		0x22
#define        REG_RX_DELAY	 			0x23
#define        REG_MAX_PAYLOAD_LENGTH 					0x23
#define        REG_OSC		 			0x24
#define        REG_HOP_PERIOD	  					0x24
#define        REG_PREAMBLE_MSB_FSK 			0x25
#define        REG_FIFO_RX_BYTE_ADDR 					0x25
#define        REG_PREAMBLE_LSB_FSK 			0x26
#define        REG_MODEM_CONFIG3	 		 		0x26
#define        REG_SYNC_CONFIG	  			0x27
#define        REG_SYNC_VALUE1	 			0x28
#define	       REG_FEI_MSB						0x28
#define        REG_SYNC_VALUE2	  			0x29
#define	       REG_FEI_MID						0x29
#define        REG_SYNC_VALUE3	  			0x2A
#define	       REG_FEI_LSB						0x2A
#define        REG_SYNC_VALUE4	  			0x2B
#define        REG_SYNC_VALUE5	  			0x2C
#define	       REG_RSSI_WIDEBAND					0x2C
#define        REG_SYNC_VALUE6	  			0x2D
#define        REG_SYNC_VALUE7	  			0x2E
#define        REG_SYNC_VALUE8	  			0x2F
#define        REG_PACKET_CONFIG1	  		0x30
#define        REG_PACKET_CONFIG2	  		0x31
#define        REG_DETECT_OPTIMIZE	  				0x31
#define        REG_PAYLOAD_LENGTH_FSK			0x32
#define        REG_NODE_ADRS	  			0x33
#define        REG_INVERT_IQ						0x33
#define        REG_BROADCAST_ADRS	 		0x34
#define        REG_FIFO_THRESH	  			0x35
#define        REG_SEQ_CONFIG1	  			0x36
#define        REG_SEQ_CONFIG2	  			0x37
#define        REG_DETECTION_THRESHOLD 					0x37
#define        REG_TIMER_RESOL	  			0x38
#define        REG_TIMER1_COEF	  			0x39
#define        REG_SYNC_WORD						0x39
#define        REG_TIMER2_COEF	  			0x3A
#define        REG_IMAGE_CAL	  			0x3B
#define        REG_TEMP		  			0x3C
#define        REG_LOW_BAT	  			0x3D
#define        REG_IRQ_FLAGS1	  			0x3E
#define        REG_IRQ_FLAGS2	  			0x3F
#define        REG_DIO_MAPPING1	  				0x40
#define        REG_DIO_MAPPING2	  				0x41
#define        REG_VERSION	  				0x42
#define        REG_PLL_HOP	  			0x44
#define        REG_TCXO		  				0x4B
#define        REG_PA_DAC	  				0x4D
#define        REG_FORMER_TEMP	  				0x5B
#define        REG_BIT_RATE_FRAC			0x5D
#define        REG_AGC_REF	  				0x61
#define        REG_AGC_THRESH1		  			0x62
#define        REG_AGC_THRESH2		  			0x63
#define        REG_AGC_THRESH3	  				0x64
#define REG_PLL	0x70


const uint32_t CH_DEFAULT = 0x6c8000; // default channel, center frequency = 434.000MHz

									  // FREQUENCY CHANNELS (BANDWIDTH 500KHz):
const uint32_t CH_1_BW_500 = 0x6c5345; // channel 1, bandwidth 500KHz, center frequency = 433.3MHz ( 433.050MHz - 433.550MHz )
const uint32_t CH_2_BW_500 = 0x6c7af3; // channel 2, bandwidth 500KHz, center frequency = 433.92MHz ( 433.670MHz - 433.920MHz )
const uint32_t CH_3_BW_500 = 0x6ca2a1; // channel 3, bandwidth 500KHz, center frequency = 434.54MHz ( 434.290MHz - 434.790MHz )

									   // FREQUENCY CHANNELS (BANDWIDTH 250KHz):
const uint32_t CH_1_BW_250 = 0x6c4b45; // channel 1, bandwidth 250KHz, center frequency = 433.175MHz ( 433.050MHz - 433.300MHz )
const uint32_t CH_2_BW_250 = 0x6c5e57; // channel 2, bandwidth 250KHz, center frequency = 433.473MHz ( 433.348MHz - 433.598MHz )
const uint32_t CH_3_BW_250 = 0x6c716a; // channel 3, bandwidth 250KHz, center frequency = 433.771MHz ( 433.646MHz - 433.896MHz )
const uint32_t CH_4_BW_250 = 0x6c847c; // channel 4, bandwidth 250KHz, center frequency = 434.069MHz ( 433.944MHz - 434.194MHz )
const uint32_t CH_5_BW_250 = 0x6c978f; // channel 5, bandwidth 250KHz, center frequency = 434.367MHz ( 434.242MHz - 434.492MHz )
const uint32_t CH_6_BW_250 = 0x6caaa1; // channel 6, bandwidth 250KHz, center frequency = 434.665MHz ( 434.540MHz - 434.790MHz )

									   // FREQUENCY CHANNELS (BANDWIDTH 125KHz):
const uint32_t CH_1_BW_125 = 0x6c4745; // channel 1, bandwidth 125KHz, center frequency = 433.1125MHz ( 433.050MHz - 433.175MHz )
const uint32_t CH_2_BW_125 = 0x6c4fe1; // channel 2, bandwidth 125KHz, center frequency = 433.247MHz ( 433.1845MHz - 433.3095MHz )
const uint32_t CH_3_BW_125 = 0x6c587c; // channel 3, bandwidth 125KHz, center frequency = 433.3815MHz ( 433.319MHz - 433.444MHz )
const uint32_t CH_4_BW_125 = 0x6c6118; // channel 4, bandwidth 125KHz, center frequency = 433.516MHz ( 433.4535MHz - 433.5785MHz )
const uint32_t CH_5_BW_125 = 0x6c69b3; // channel 5, bandwidth 125KHz, center frequency = 433.6505MHz ( 433.588MHz - 433.713MHz )
const uint32_t CH_6_BW_125 = 0x6c724f; // channel 6, bandwidth 125KHz, center frequency = 433.785MHz ( 433.7225MHz - 433.8475MHz )
const uint32_t CH_7_BW_125 = 0x6c7af3; // channel 7, bandwidth 125KHz, center frequency = 433.92MHz ( 433.8575MHz - 433.9825MHz )
const uint32_t CH_8_BW_125 = 0x6c8397; // channel 8, bandwidth 125KHz, center frequency = 434.055MHz ( 433.9925MHz - 434.1175MHz )
const uint32_t CH_9_BW_125 = 0x6c8c32; // channel 9, bandwidth 125KHz, center frequency = 434.1895MHz ( 434.127MHz - 434.252MHz )
const uint32_t CH_10_BW_125 = 0x6c94ce; // channel 10, bandwidth 125KHz, center frequency = 434.324MHz ( 434.2615MHz - 434.3865MHz )
const uint32_t CH_11_BW_125 = 0x6c9d6a; // channel 11, bandwidth 125KHz, center frequency = 434.4585MHz ( 434.396MHz - 434.521MHz )
const uint32_t CH_12_BW_125 = 0x6ca605; // channel 12, bandwidth 125KHz, center frequency = 434.593MHz ( 434.5305MHz - 434.6555MHz )
const uint32_t CH_13_BW_125 = 0x6CaeA1; // channel 13, bandwidth 125KHz, center frequency = 434.7275MHz ( 434.665MHz - 434.790MHz )

										// FREQUENCY CHANNELS (BANDWIDTH < 125KHz: separate 72.5KHz):
const uint32_t CH_1 = 0x6c4597; // channel 1, center freq = 433.086MHz
const uint32_t CH_2 = 0x6c4a3b; // channel 2, center freq = 433.159MHz
const uint32_t CH_3 = 0x6c4edf; // channel 3, center freq = 433.231MHz
const uint32_t CH_4 = 0x6c5383; // channel 4, center freq = 433.304MHz
const uint32_t CH_5 = 0x6c5827; // channel 5, center freq = 433.376MHz
const uint32_t CH_6 = 0x6c5ccb; // channel 6, center freq = 433.449MHz
const uint32_t CH_7 = 0x6c616f; // channel 7, center freq = 433.521MHz
const uint32_t CH_8 = 0x6c6613; // channel 8, center freq = 433.594MHz
const uint32_t CH_9 = 0x6c6ab7; // channel 9, center freq = 433.666MHz
const uint32_t CH_10 = 0x6c6f5b; // channel 10, center freq = 433.739MHz
const uint32_t CH_11 = 0x6c73ff; // channel 11, center freq = 433.811MHz
const uint32_t CH_12 = 0x6c78a3; // channel 12, center freq = 433.884MHz
const uint32_t CH_13 = 0x6c7d47; // channel 13, center freq = 433.956MHz
const uint32_t CH_14 = 0x6c81eb; // channel 14, center freq = 434.029MHz
const uint32_t CH_15 = 0x6c868f; // channel 15, center freq = 434.101MHz
const uint32_t CH_16 = 0x6c8b33; // channel 16, center freq = 434.174MHz
const uint32_t CH_17 = 0x6c8fd8; // channel 17, center freq = 434.246MHz
const uint32_t CH_18 = 0x6c947c; // channel 18, center freq = 434.319MHz
const uint32_t CH_19 = 0x6c9920; // channel 19, center freq = 434.391MHz
const uint32_t CH_20 = 0x6c9dc4; // channel 20, center freq = 434.464MHz
const uint32_t CH_21 = 0x6ca268; // channel 21, center freq = 434.536MHz
const uint32_t CH_22 = 0x6ca70c; // channel 22, center freq = 434.609MHz
const uint32_t CH_23 = 0x6cabb0; // channel 23, center freq = 434.682MHz
const uint32_t CH_24 = 0x6cb054; // channel 24, center freq = 434.754MHz

								 //LORA BANDWIDTH:
const uint8_t BW_7_8 = 0x00;
const uint8_t BW_10_4 = 0x01;
const uint8_t BW_15_6 = 0x02;
const uint8_t BW_20_8 = 0x03;
const uint8_t BW_31_2 = 0x04;
const uint8_t BW_41_7 = 0x05;
const uint8_t BW_62_5 = 0x06;
const uint8_t BW_125 = 0x07;
const uint8_t BW_250 = 0x08;
const uint8_t BW_500 = 0x09;

const double SignalBwLog[] =
{
	5.0969100130080564143587833158265,
	5.397940008672037609572522210551,
	5.6989700043360188047862611052755
};

//LORA CODING RATE:
const uint8_t CR_5 = 0x01;	// CR = 4/5
const uint8_t CR_6 = 0x02;	// CR = 4/6
const uint8_t CR_7 = 0x03;	// CR = 4/7
const uint8_t CR_8 = 0x04;	// CR = 4/8

							//LORA SPREADING FACTOR:
const uint8_t SF_6 = 0x06;
const uint8_t SF_7 = 0x07;
const uint8_t SF_8 = 0x08;
const uint8_t SF_9 = 0x09;
const uint8_t SF_10 = 0x0A;
const uint8_t SF_11 = 0x0B;
const uint8_t SF_12 = 0x0C;

//LORA MODES:
const uint8_t LORA_SLEEP_MODE = 0x80;
const uint8_t LORA_STANDBY_MODE = 0x81;
const uint8_t LORA_TX_MODE = 0x83;
const uint8_t LORA_RX_MODE = 0x85;
const uint8_t LORA_STANDBY_FSK_REGS_MODE = 0xC1;

//FSK MODES:
const uint8_t FSK_SLEEP_MODE = 0x00;
const uint8_t FSK_STANDBY_MODE = 0x01;
const uint8_t FSK_TX_MODE = 0x03;
const uint8_t FSK_RX_MODE = 0x05;

//OTHER CONSTANTS:

const uint8_t st_SD_ON = 1;
const uint8_t st_SD_OFF = 0;
const uint8_t HEADER_ON = 0;
const uint8_t HEADER_OFF = 1;
const uint8_t CRC_ON = 1;
const uint8_t CRC_OFF = 0;
const uint8_t LORA = 1;
const uint8_t FSK = 0;
const uint8_t BROADCAST_0 = 0x00;
const uint8_t MAX_LENGTH = 255;
const uint8_t MAX_PAYLOAD = 251;
const uint8_t MAX_LENGTH_FSK = 64;
const uint8_t MAX_PAYLOAD_FSK = 60;
const uint8_t ACK_LENGTH = 5;
const uint8_t OFFSET_PAYLOADLENGTH = 5;
const uint8_t OFFSET_RSSI = 137;
const uint8_t NOISE_FIGURE = 6.0;
const uint8_t NOISE_ABSOLUTE_ZERO = 174.0;
const uint16_t MAX_TIMEOUT = 10000;		//10000 msec = 10.0 sec
const uint32_t MAX_WAIT = 12000;		//12000 msec = 12.0 sec
const uint32_t MESH_TIMEOUT = 3600000;  //3600000 msec = 3600 sec = 1 hour
const uint8_t MAX_RETRIES = 5;
const uint8_t CORRECT_PACKET = 0;
const uint8_t INCORRECT_PACKET = 1;

//! Structure :
/*!
*/
#define MAX_PAYLOAD 0
struct pack
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
};

#endif