/******************************************************************************
* Filename     : conf.h                                                       *
* Program      : Private LoRa                                                 *
* Copyright    : Copyright (C) 2017-2017, ETRI                                *
*              : URL << http://www.etri.re.kr >>                              *
* Authors      : Woo-Sung Jung (woosung@etri.re.kr)                           *
*              : Tae Hyun Yoon (thyoon0820@etri.re.kr)                        *
*              : Dae Seung Yoo (ooseyds@etri.re.kr)                           *
* Description  : Private LoRa Configuration Setting                           *
* Created at   : Mon Mar 27 2017.                                             *
* Modified by  :                                                              *
* Modified at  :                                                              *
******************************************************************************/

#ifndef __CONF_H_
#define __CONF_H_

/*
 * Configuration element
 * 1. Node Type
 * 2. RF Frequency
 * 3. Transmission Power
 * 4. LoRa Parameter
 * 5. Device ID
 * 5. Interface for other sensor devices (e.g. UWB location module)?????
 * 6. Application Parameters
 * 7. Experiment Parameters
 */

// Node Type..

// 1. Node Type  -----------------------------------------------------------------
#define LoRaGateWay
//#define UWB_SERVICE
#define DEBUG_ENABLED

/* lam.hq: Clock definition - This is used for the modified system clock tick of RTOS
 * The new granularity is 100 us instead of default value 1 ms
 * Change the value of portTICK_PERIOD_MS in portmacro.h and
 * configTICK_RATE_HZ in FreeRTOSConfig.h to respective value
 */
#define TIMEBASE									1	// 1 TIMEBASE is equal to 1000us
#define MILISECOND									(TIMEBASE*1)
#define SECOND										(MILISECOND*1000)

#if defined (LoRaGateWay)
#undef LoRaEndDevice
#else
#define LoRaEndDevice
#undef LoRaDateWay
#endif


// 2. RF Frequency  --------------------------------------------------------------
#if defined( USE_BAND_433 )
#define RF_FREQUENCY                                434000000 // Hz
#elif defined( USE_BAND_780 )
#define RF_FREQUENCY                                780000000 // Hz
#elif defined( USE_BAND_868 )
#define RF_FREQUENCY                                868000000 // Hz
#elif defined( USE_BAND_915 )
//#define RF_FREQUENCY                                915000000 // Hz
#define RF_FREQUENCY_1                                919700000 // Hz	channel 32 in 917-923.5MHz
#define RF_FREQUENCY_2                                919900000 // Hz	channel 32 in 917-923.5MHz
#define RF_FREQUENCY_3                                920100000 // Hz	channel 32 in 917-923.5MHz
#define RF_FREQUENCY_4                                920300000 // Hz	channel 32 in 917-923.5MHz
#else
    #error "Please define a frequency band in the compiler options."
#endif


// 3. Transmission Power  ---------------------------------------------------------
#define TX_OUTPUT_POWER                             14        // dBm


// 4. LoRa Parameter  -------------------------------------------------------------
#if defined( USE_MODEM_LORA )
#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE 			                1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         5         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#define LoRa_SYNCWORD								0x34	  // Semtech default  0x12
                                                              // LoRaWAN SyncWord 0x34

#elif defined( USE_MODEM_FSK )
#define FSK_FDEV                                    25e3      // Hz
#define FSK_DATARATE                                50e3      // bps
#define FSK_BANDWIDTH                               50e3      // Hz
#define FSK_AFC_BANDWIDTH                           83.333e3  // Hz
#define FSK_PREAMBLE_LENGTH                         5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON                   false

#else
    #error "Please define a modem in the compiler options."
#endif

// KR920-923 Default Settings
#define	RECEIVE_DELAY1		1000		// 1 sec
#define	RECEIVE_DELAY2		2000		// 2 sec
#define	JOIN_ACCEPT_DELAY1	5000		// 5 sec
#define	JOIN_ACCEPT_DELAY2	6000		// 6 sec
#define	MAX_FCNT_GAP		16384
#define	ADR_ACK_LIMIT		64000		// 64 sec
#define ADR_ACK_DELAY		32000		// 32 sec
#define	ACK_TIMEOUT			2000		// 2 sec

#define LORAWAN_R1			0			// LoRaWAN R1 value

#define RX_TIMEOUT_VALUE                            RECEIVE_DELAY1
#define BUFFER_SIZE                                 256 //  64 -> 256 Define the payload size here
#define U1_RXBUFFERSIZE								256	// 512 -> 256


// 5. Device ID  ------------------------------------------------------------------
#if defined (LoRaGateWay)
#define DEV_ID										0x00	// Gateway 0x0000 / End Device 0x0101 ~
#else
#define DEV_ID										0x02// Gateway 0x0000 / End Device 0x0101 ~
#endif

#define Nwk_ID										0x0001	// [16..12]
#define Nwk_Addr									DEV_ID	// [11..00]
// 5. Interfaces  ------------------------------------------------------------------
//#define UWB_SERVICE									// Gateway 0x0001 / End Device 0x0101 ~


// 6. Application Params  ----------------------------------------------------------
#define APP_PORT									2		// 1..223
#define ACK_REQ										0		// unconfirmed data
#define MONITORING_DATA_INTERVAL					10000	// 5000 ms
															// Current Max Tick Value is 65535 (uint16_t)
// Value from UWB Service & IP Tunneling
#define START_OF_FRAME								0xDE
#define END_OF_FRAME								0xCA
#define AUTOSTART

// Only one of application have to define..
//#define UBICOM_APP
//#define RWSMS_ON
//#define GDD_ON
//#define TESTAPP_ON

#ifdef UBICOM_APP
	#undef RWSMS_ON
	#undef GDD_ON
	#undef TESTAPP_ON
#endif
#ifdef RWSMS_ON
	#undef GDD_ON
	#undef TESTAPP_ON
	#undef UBICOM_APP
#endif
#ifdef GDD_ON
	#undef RWSMS_ON
	#undef TESTAPP_ON
	#undef UBICOM_APP
#endif
#ifdef TESTAPP_ON
	#undef RWSMS_ON
	#undef GDD_ON
	#undef UBICOM_APP
#endif

// 7. Experiment Params  -----------------------------------------------------------
#define EXPERIMENT
#define LED_BLINK
#if defined (LoRaGateWay)
#define BLINK_INTERVAL								3000
#else
#define BLINK_INTERVAL								500
#endif


// 8. For debug
//#define ENCRYPT_OFF_DEBUG
//#define ADHOC_CONNECTION

// 9. RMSS channel setting
#define NUMBER_OF_SELECTTED_CHANNELS				1
#define NUMBER_OF_AVAILABLE_CHANNELS				1
#define CHANNEL_HOPPING_ENABLE						0

// 10. Define some boolean macro
#define TRUE										1
#define FALSE										0

// 11. Network struture parameters
#define NUMBER_OF_NETWORK_FORMING_BEACON			3
#endif // end of  __CONF_H_
