/******************************************************************************
* Filename     : RMSS_lora_mac.h											  *
* Program      : RMSS LoRa MAC protocol			                          *
* Authors      : Quy Lam Hoang 						                          *
* Description  : RMSS LoRa MAC protocol 			                  *
* Created at   : Nov 17 2017.                                            	  *
* Modified by  :                                                              *
* Modified at  :                                                              *
******************************************************************************/

#ifndef RMSS_LORA_MAC_H_
#define RMSS_LORA_MAC_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <conf.h>


#define BITSET				1
#define BITUNSET			0
#define TUNNELING_OVERHEAD		10

// Group and channel setting
#define NUMBER_OF_GROUP	NUMBER_OF_SELECTTED_CHANNELS

// Variable definition
static uint16_t BcCounter;
static uint8_t currentCodingRate;
static uint8_t currentSpradingFactor;
/*!
 * \LoRa MAC Initialization codes
 */
void RmssMacInit(void);

/******************************************************************************
* Function Name        : IsGateway
* Input Parameters     : None
* Return Value         : Device Type : 1 - Gateway / 0 - End Device
* Function Description : Return Device Type
******************************************************************************/
uint8_t IsGateway(void);

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
void OnTxDone( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
void OnTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
void OnRxTimeout( void );

/*!
 * \brief Function executed on Radio Rx Error event
 */
void OnRxError( void );


/******************************************************************************
* Function Name        : SendFrame
* Input Parameters     : uint8_t *buffer   - Application payload
*                      : uint8_t size      - Application Size
*                      : uint8_t port      - LoRa Frame Port
*                      :                     Port = 0 (Only MAC command)
*                      :                     Port = 1..223 (Application Specific)
*                      :                     Port = 224 (LoRaWAN Test Protocol)
*                      : uint8_t ack_req   - Confirmed data(1) or Unconfirmed data(0)
*                      : uint8_t direction - UpLink (0) or DownLink (1)
* Return Value         : None
* Function Description : Generate LoRa MAC Frame and Send it
******************************************************************************/
void RmssSendFrame(uint8_t *buffer, uint8_t size, uint8_t port, uint8_t ack_req, uint8_t direction);

#ifdef LoRaGateWay
/******************************************************************************
* Function Name        : RmssSendBeacon
* Input Parameters     : RmssBcType_t bcType - Type of Beacon message
* Return Value         : None
* Function Description : Generate LoRa MAC Beacon and Send it
******************************************************************************/
void RmssSendBeacon(RmssBcType_t bcType);
#endif

/******************************************************************************
* Function Name        : SendJoinRequest
* Input Parameters     : None
* Return Value         : None
* Function Description : The join procedure requires the end-device to be
*                      : personalized with the following information before its
*                      : starts the join procedure: a globally unique end-device
*                      : identifier (DevEUI), the application identifier (AppEUI),
*                      : and an AES-128 key (AppKey)
******************************************************************************/
void SendJoinRequest(void);

/******************************************************************************
* Function Name        : SendFrameGateway
* Input Parameters     : uint8_t *buffer   - LoRa Frame
*                      : uint8_t size      - LoRa Frame Size
* Return Value         : None
* Function Description : Send LoRa MAC Frame @ Gateway
******************************************************************************/
void SendFrameGateway(uint8_t *buffer, uint8_t size);

/******************************************************************************
* Function Name        : DelayedSendFrame
* Input Parameters     : None
* Return Value         : None
* Function Description : This function is called by Timer expired IRQ
******************************************************************************/
void DelayedSendFrame(void);

/******************************************************************************
* Function Name        : ReceiveFrame
* Input Parameters     : None
* Return Value         : None
* Function Description : When LoRa MAC status change to RX,
*                      : LoRaMacStatusUpdate calls it.
*                      : It can handle both gateway and end device.
******************************************************************************/
void ReceiveFrame(void);

/******************************************************************************
* Function Name        : LoRaMacStatusUpdate
* Input Parameters     : None
* Return Value         : None
* Function Description : Periodic status check call by main loop pooling
*                      : Only Class A is supported
******************************************************************************/
void LoraMacStatusUpdate(void);

/******************************************************************************
* Function Name        : IsNetworkJoined
* Input Parameters     : None
* Return Value         : 1 - End Device is joined LoRa network
*                      : 0 - End Device is not joined
* Function Description : Network Join status check..
******************************************************************************/
uint8_t IsNetworkJoined(void);

/* The enum define the beacon msg type which is sent by the gateway */
typedef enum RmssBcType {
	RMSS_BC_NW_INITILIZATION			= 0x00,
	RMSS_BC_START_FRAME					= 0x01,
}RmssBcType_t;

typedef struct RmssMacBcHeader {
	union {
		RmssBcType_t	bcType				:2;		//beacon type
		uint8_t			groupIndex			:3;
		uint8_t			isContainSchedule	:1;		// 1: BC contains the schedule for groupIndex
													// 0: no schedule is contained
		uint8_t			RFU					:2;
	}BcFrameHeader;
	uint16_t gwAddress;								// gateway address
	union {
		uint16_t		superFrameCnt		:12;	// count the number of superframe
		uint16_t		frameCnt			:4;		// count the number of frame in each superframe
	}FrameCount;
}RmssMacBcHeader_t;


typedef struct RmssMacStatus {
	uint8_t DeviceType;			// 1 - GateWay / 0 - End device
	uint8_t Joined;				// 1 - Joined / 0 - others
	uint8_t ADRACKRequested;	// 1 - Network server Request ADRACK / 0 - others
	uint8_t ACKRequested;		// 1 - Network server Request ACK / 0 - others
	uint8_t FPending;			// 1 - more data is waiting.. / 0 - no more data
	uint8_t ADR;
}RmssMacStatus_t;

typedef union MacHeader {
	uint8_t value;
	struct MacHeader_value{
		uint8_t RFU		: 4;
		uint8_t MType	: 4;
	}bits;
}MacHeader_t;

typedef struct RmssMacHeader {

	MacHeader_t macHeader;
	union {
		uint16_t Address;
		RMSSDevAddr_t Bit;
	}DevAddr;
	uint16_t Fcnt;

}RmssMacHeader_t;

typedef enum RMSSDevType {
	RMSS_MAC_EndDevice		= 0x00,
	RMSS_MAC_Gateway		= 0x01,
}RMSSDevType_t;

typedef enum RMSSMacStatusConnection {
	RMSS_MAC_NotJoined		= 0x00,
	RMSS_MAC_Joined			= 0x01,
}RMSSMacStatusConnection_t;


typedef enum RMSSDirection {
	RMSS_UP_LINK			= 0x00,
	RMSS_DN_LINK			= 0x01,
}RMSSDirection_t;

// Chapter 4.2.1 Message Type (MType bit field)
typedef enum RMSSMACFrameType{
	RMSS_Frame_Join_Request			= 0x00,
	RMSS_Frame_Join_Accept			= 0x01,
	RMSS_Frame_Unconfirm_Data_Up	= 0x02,
	RMSS_Frame_Unconfirm_Data_Down	= 0x03,
	RMSS_Frame_Confirm_Data_Up		= 0x04,
	RMSS_Frame_Confirm_Data_Down	= 0x05,
	RMSS_Frame_RFU					= 0x06,
	RMSS_Frame_Proprietary			= 0x07,
}RMSSMACFrameType_t;

// Chapter 4.2.1 Message Type (MType bit field)
typedef enum RMSSNodePriorityLevel{
	RMSS_Node_Priority_Level_1			= 0x01,
	RMSS_Node_Priority_Level_2			= 0x02,
	RMSS_Node_Priority_Level_3			= 0x03,
	RMSS_Node_Priority_Level_4			= 0x04,
	RMSS_Node_Priority_Level_5			= 0x05,
	RMSS_Node_Priority_Level_6			= 0x06,
}RMSSNodePriorityLevel_t;

/*
// Chapter 5 MAC Commands
typedef enum RMSSMACCommandEndDevice {
	LoRa_MAC_CMD_LinkCheckReq		= 0x02,
	LoRa_MAC_CMD_LinkADRAns			= 0x03,
	LoRa_MAC_CMD_DutyCycleAns		= 0x04,
	LoRa_MAC_CMD_RxParamSetupAns	= 0x05,
	LoRa_MAC_CMD_DevStatusAns		= 0x06,
	LoRa_MAC_CMD_NewChannelAns		= 0x07,
	LoRa_MAC_CMD_RxTimingSetupAns	= 0x08,
	LoRa_MAC_CMD_TxParamSetupAns	= 0x09,
	LoRa_MAC_CMD_DIChannelAns		= 0x0A,
}LoRaMACCommandEndDevice_t;


typedef enum LoRaMACCommandGW {
	LoRa_MAC_CMD_LinkCheckAns		= 0x02,
	LoRa_MAC_CMD_LinkADRReq			= 0x03,
	LoRa_MAC_CMD_DutyCycleReq		= 0x04,
	LoRa_MAC_CMD_RxParamSetupReq	= 0x05,
	LoRa_MAC_CMD_DevStatusReq		= 0x06,
	LoRa_MAC_CMD_NewChannelReq		= 0x07,
	LoRa_MAC_CMD_RxTimingSetupReq	= 0x08,
	LoRa_MAC_CMD_TxParamSetupReq	= 0x09,
	LoRa_MAC_CMD_DIChannelReq		= 0x0A,
}LoRaMACCommandGW_t;
*/
typedef struct RMSSRxFrameInfo{
	uint8_t startOfFrame;
	uint8_t frameSize;
	int16_t rssi;
	int8_t snr;
	uint8_t cr;
	uint8_t sf;
	uint8_t appPayloadSize;
	uint8_t startOfAppPayload;
	uint8_t payload[BUFFER_SIZE];
	uint8_t endOfFrame;
}RMSSRxFrameInfo_t;

#endif /* SLOTTED_LORA_MAC_H_ */
