/******************************************************************************
* Filename     : app.h                                                        *
* Program      : Private LoRa Application                                     *
* Copyright    : Copyright (C) 2017-2017, ETRI                                *
*              : URL << http://www.etri.re.kr >>                              *
* Authors      : Woo-Sung Jung (woosung@etri.re.kr)                           *
*              : Tae Hyun Yoon (thyoon0820@etri.re.kr)                        *
*              : Dae Seung Yoo (ooseyds@etri.re.kr)                           *
* Description  : Private LoRa Application Definition                          *
* Created at   : Mon July 24 2017.                                            *
* Modified by  :                                                              *
* Modified at  :                                                              *
******************************************************************************/

#ifndef __APP_H_
#define __APP_H_

#include <conf.h>
#include <main.h>

// TESTING
typedef struct
{
	uint16_t	ID;
	uint16_t	SeqNum;
	uint16_t	TxPwr;
	uint8_t		SF;
	uint8_t		CR;
	uint32_t	Time;
	uint32_t	Time2;			// Packet Size = 16
	uint8_t		pading[48];		// Total Packet Size = 64
}App_t;

/*
Data Transmission (Msg ID = 1)
  1  2  3   4  5  6  7  8  9 10  11 12 13 14 15 16 17 18 19 20
+---------+-----+---+-------+---+--------------+-----+--------+
| start   | seq |msg| ppp ID|ws |  PPE Data    |reser|  end   |
|         |     |ID |       |ID | 1| 2| 3| 4| 5|ved  |        |
+---------+-----+---+-------+---+--------------+-----+--------+
| m  i  c |00-99| 1 |000-999|1-4|     0 - 3    | 0 0 | fin    |
+---------+-----+---+-------+---+--------------+-----+--------+

Rule Control (Msg ID = 2)
  1  2  3   4  5  6  7  8  9 10  11 12 13 14 15 16 17 18 19 20
+---------+-----+---+-------+---+--------------+-----+--------+
| start   | seq |msg| ppp ID|ws |  PPE Rule    |reser|  end   |
|         |     |ID |       |ID | 1| 2| 3| 4| 5|ved  |        |
+---------+-----+---+-------+---+--------------+-----+--------+
| m  i  c |00-99| 1 |000-999|1-4|     0 - 3    | 0 0 | fin    |
+---------+-----+---+-------+---+--------------+-----+--------+

Interval Setup (Msg ID = 3)
  1  2  3   4  5  6  7  8  9 10  11 12 13 14 15 16 17 18 19 20
+---------+-----+---+-------+---+--------------+-----+--------+
| start   | seq |msg| ppp ID|ws |  Data Send   |reser|  end   |
|         |     |ID |       |ID |  Interval    |ved  |        |
+---------+-----+---+-------+---+--------------+-----+--------+
| m  i  c |00-99| 1 |000-999|1-4|  0 - 99999   | 0 0 | fin    |
+---------+-----+---+-------+---+--------------+-----+--------+

LED, Buzzer (Msg ID = 4)
  1  2  3   4  5  6  7  8  9 10  11 12 13 14 15 16 17 18 19 20
+---------+-----+---+-------+---+--+--+--------------+--------+
| start   | seq |msg| ppp ID|ws | L| B| reserved     |  end   |
|         |     |ID |       |ID |  |  |              |        |
+---------+-----+---+-------+---+--+--+--------------+--------+
| m  i  c |00-99| 1 |000-999|1-4| 0-1 | 0  0  0  0 0 | fin    |
+---------+-----+---+-------+---+-----+--------------+--------+

ACK (Msg ID = 9)
  1  2  3   4  5  6  7  8  9 10  11 12 13 14 15 16 17 18 19 20
+---------+-----+---+-------+---+--------------+-----+--------+
| start   | seq |msg| ppp ID|ws |    ACK       |reser|  end   |
|         |     |ID |       |ID | seq |retrycnt|ved  |        |
+---------+-----+---+-------+---+--------------+-----+--------+
| m  i  c |00-99| 1 |000-999|1-4|0-99 | 1 - 2  | 0 0 | fin    |
+---------+-----+---+-------+---+--------------+-----+--------+

*/

#define APP_PAYLOAD_LENGTH						15
typedef struct UbicomApp{
        uint8_t appID;
        uint8_t msgID;
        uint16_t seqNum;
        uint8_t data[APP_PAYLOAD_LENGTH];
}UbicomApp_t;

typedef struct RWSMS{
        uint8_t seqNum;         // 2 digits
        uint8_t msgID;          // 1 digit
        uint16_t pppID;         // 3 digits
        uint8_t wsID;           // 1 digit
        uint8_t ppeData[5];     // 5 digits only if msgID = 1
        uint8_t ppeRule[5];     // 5 digits only if msgID = 2
        uint32_t interval;      // 5 digits only if msgID = 3
        uint8_t led;            // 1 digit  only if msgID = 4
        uint8_t buzzer;         // 1 digit  only if msgID = 4
        uint8_t ackSeq;         // 2 digits only if msgID = 5
        uint16_t ackRetryCnt;   // 3 digits only if msgID = 5
}RWSMS_t;

typedef enum app_type {
        UBICOM_APP_TYPE = 1,
		RWSMS_APP_TYPE = 2,
		GDD_APP_TYPE = 3,
}app_type_t;

typedef enum message_type {
        DATA_MSG = 1,
        RULE_MSG = 2,
        INTERVAL_MSG = 3,
        LED_BUZZER_MSG = 4,
        ACK_MSG = 9,
}message_type_t;

/*
Gas Detection Device Message Format
  [GAS:VALUE|GAS2:VALUE2|...]
  Value Range = -99.9 ~ 99.9

Example
  [O2: 20.9|CO: 00.3|H2: 00.2|H2S: 00.7|CH4: -84.3]

 */

// For the elimination of float computing, unit is 1.
typedef struct GDD {
        int16_t oxygen;             // O2
        int16_t carbonOxygen;       // CO
        int16_t hydrogen;           // H2
        int16_t hydrogenSulfide;    // H2S
        int16_t methane;            // CH4
}GDD_t;


/******************************************************************************
* Function Name        : ApplicationInit XXX
* Input Parameters     : Each Application Pointer
* Return Value         : None
* Function Description : Application Value Initialization..
******************************************************************************/
#if defined (UBICOM_APP)
	void ApplicationInitUbicom(UbicomApp_t *app);
	void ApplicationGenerationUbicom(UbicomApp_t *app, uint8_t *tempbuffer);
#elif defined (RWSMS_ON)
	void ApplicationInitRWSMS(RWSMS_t	*app);
	void ApplicationGenerationRWSMS(RWSMS_t *app, uint8_t *tempbuffer);
	void ApplicationRxHandlerRWSMS(uint8_t *rxPayload);
#elif defined (GDD_ON)
	void ApplicationInitGDD(GDD_t	*app);
	void ApplicationGenerationGDD(GDD_t *app, uint8_t *tempbuffer);
#else
	void ApplicationInitTemp(App_t	*app);
#endif

/******************************************************************************
* Function Name        : GetAppKey
* Input Parameters     : Nonce
* Return Value         : Application Key Value
* Function Description : Application Key Value return
******************************************************************************/
void GetAppKey(uint8_t *key);

#endif // __APP_H_
