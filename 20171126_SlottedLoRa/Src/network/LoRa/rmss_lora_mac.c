/******************************************************************************
* Filename     : rmss_lora_mac.c											  *
* Program      : RMSS LoRa MAC protocol			                          *
* Authors      : Quy Lam Hoang 						                          *
* Description  : Slotted LoRa MAC protocol 			                  *
* Created at   : Nov 17 2017.                                            	  *
* Modified by  :                                                              *
* Modified at  :                                                              *
******************************************************************************/

#include <string.h>
#include <radio.h>
#include <sx1276.h>
#include <sx1276-ubiboard.h>
#include <vcp.h>
#include <delay.h>
#include <conf.h>
#include <rmss_lora_mac.h>
#include <timer.h>

// Global Variables
static RadioEvents_t RadioEvents;
uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];
States_t State = LOWPOWER;

int16_t RssiValue = 0;
int8_t SnrValue = 0;
//uint16_t FCntUp;
uint8_t tx_information[256];	// Debug msg via VCP

RMSSDevAddr_t devAddr;
RMSSMacStatus_t MacStatus;
RMSSRxFrameInfo_t rxframe;
uint8_t rxAppData[256];		// using for Decryption..

// Handling by Main routine...
extern void RxHandlerDevice(RMSSRxFrameInfo_t);
extern void RxHandlerGW(RMSSRxFrameInfo_t);

/******************************************************************************
* Function Name        : RMSSMAC_Init
* Input Parameters     : None
* Return Value         : None
* Function Description : Slotted LoRa MAC Initialization routine
******************************************************************************/
void RmssMacInit(void)
{
	// semtech sx1276 initialization
	SX1276IoInit( );

	RadioEvents.TxDone = OnTxDone;
	RadioEvents.RxDone = OnRxDone;
	RadioEvents.TxTimeout = OnTxTimeout;
	RadioEvents.RxTimeout = OnRxTimeout;
	RadioEvents.RxError = OnRxError;

	Radio.Init( &RadioEvents );

	// Use this channel at the bootstrap phase for network forming
	Radio.SetChannel( RF_FREQUENCY_1 );

	currentCodingRate = LORA_CODINGRATE;
	// TO DO: change to SF7 or SF8
	currentSpradingFactor = LORA_SPREADING_FACTOR;

	// Initialize Delayed Tx Timer
	//TimerInit( &TxBackoffTimer, DelayedSendFrame );
	//TimerSetValue( &TxBackoffTimer, timeout );
	//TimerStart( &TxBackoffTimer );

#if defined( USE_MODEM_LORA )
	Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
									currentSpradingFactor, currentCodingRate,
									LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
									true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

	Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, currentSpradingFactor,
									currentCodingRate, 0, LORA_PREAMBLE_LENGTH,
									LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
									0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

#elif defined( USE_MODEM_FSK )
	Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
									FSK_DATARATE, 0,
									FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
									true, 0, 0, 0, 3000 );

	Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
									0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
									0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
									0, 0,false, true );

#else
	#error "Please define a frequency band in the compiler options."
#endif

	// TODO:: Timer will be Added this code.. -> TxScheduling / ACK Timeout / Rx1 / Rx2

	// Address
	devAddr.NwkID = Nwk_ID;
	devAddr.NwkAddr = Nwk_Addr;

	BcCounter = 0;

	// Mac Status
#if defined (LoRaGateWay)
	MacStatus.DeviceType = RMSS_MAC_Gateway;
	MacStatus.Joined = RMSS_MAC_Joined;
#else
	MacStatus.DeviceType = RMSS_MAC_EndDevice;
	MacStatus.Joined = RMSS_MAC_NotJoined;
#endif

}

/******************************************************************************
* Function Name        : IsGateway
* Input Parameters     : None
* Return Value         : Device Type : 1 - Gateway / 0 - End Device
* Function Description : Return Device Type
******************************************************************************/
uint8_t IsGateway(void)
{
	return MacStatus.DeviceType;
}

/******************************************************************************
* Function Name        : OnTxDone
* Input Parameters     : None
* Return Value         : None
* Function Description : PHY reports Tx done event
******************************************************************************/
void OnTxDone( void )
{
#if defined(LoRaGateWay)
	//Gateway Tx done.. waiting Rx continuously
	Radio.Rx( 0 );
#else
	//End device will wait Rx Frame...
	//Radio.Rx( RX_TIMEOUT_VALUE );
	Radio.Rx( 0 );
#endif
    State = TX;
}

/******************************************************************************
* Function Name        : OnRxDone
* Input Parameters     : uint8_t *payload  - Received LoRa Frame
*                      : uint8_t size      - Application Size
*                      : int16_t rssi      - Received Signal Strength Indicator
*                      : int8_t snr        - Signal to Noise Ratio
* Return Value         : None
* Function Description : PHY reports Rx done event
*                      : Copy Rx Frame to prevent buffer overwrite
******************************************************************************/
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
	// TODO:: Multiple FIFO is required..?
#if defined (LoRaGateWay)
#else
    //Radio.Sleep( );
#endif

    rxframe.startOfFrame = START_OF_FRAME;
    rxframe.rssi = rssi;
    rxframe.snr = snr;
    rxframe.frameSize = (uint8_t) (size &0xff);
    memset(rxframe.payload, 0, sizeof(rxframe.payload));
    memcpy(rxframe.payload, payload, rxframe.frameSize);
    rxframe.endOfFrame = END_OF_FRAME;

    // TODO:: How to Get this information when we using SX1301?
	rxframe.sf = currentSpradingFactor;				// How to get?
	rxframe.cr = currentCodingRate;					// How to get?

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
    State = RX;
}

/******************************************************************************
* Function Name        : OnTxTimeout
* Input Parameters     : None
* Return Value         : None
* Function Description : PHY reports Tx Time event, No more data to send
******************************************************************************/
void OnTxTimeout( void )
{
#if defined (LoRaGateWay)
#else
    //Radio.Sleep( );
#endif
    State = TX_TIMEOUT;
}

/******************************************************************************
* Function Name        : OnRxTimeout
* Input Parameters     : None
* Return Value         : None
* Function Description : PHY reports Rx Time event, move second slot or sleep
******************************************************************************/
void OnRxTimeout( void )
{
#if defined (LoRaGateWay)
#else
    //Radio.Sleep( );
#endif
    if(State == RX_WAITING1)
    	State = RX_TIMEOUT_WAITING1;
    else if(State == RX_WAITING2)
    	State = RX_TIMEOUT_WAITING2;
    else
    	State = RX_ERROR;
}

/******************************************************************************
* Function Name        : OnRxError
* Input Parameters     : None
* Return Value         : None
* Function Description : PHY reports Rx Error event
******************************************************************************/
void OnRxError( void )
{
    //Radio.Sleep( );
    State = RX_ERROR;
}

void RmssSendFrame(uint8_t *buffer, uint8_t size, uint8_t port, uint8_t ack_req, uint8_t direction)
{
	// TODO:: ALOHA RANDOM DELAY will be added.. Also 4 sec LBT...
	uint8_t lorabuffer[256];
	uint8_t lorasize = 0;
	RMSSMacHeader_t fHeader;
	//uint32_t mic = 0x12345678;	// TESTING

	// Up-Link (Device -> GW)
	if(direction == RMSS_UP_LINK)
	{
		memset(lorabuffer, 0 , 256);

		if(ack_req == 0)
		{
			//Unconfirmed Message
			fHeader.macHeader.bits.MType = RMSS_Frame_Unconfirm_Data_Up;
		}
		else
		{
			//Confirmed Message
			fHeader.macHeader.bits.MType = RMSS_Frame_Confirm_Data_Up;
		}

		memcpy(&lorabuffer[lorasize], &fHeader.macHeader, 1);
		lorasize += 1;

		// Generate Frame Header.. DevAddr(4) FCtrl(1) FCnt(2) FOpts(0..15)
		fHeader.DevAddr.Bit.NwkAddr = Nwk_Addr;
		fHeader.DevAddr.Bit.NwkID = Nwk_ID;

		memcpy(&lorabuffer[lorasize], &fHeader.DevAddr.Address, 2);
		lorasize += 2;

		fHeader.Fcnt = FCntUp++;
		memcpy(&lorabuffer[lorasize], &fHeader.Fcnt, 2);
		lorasize += 2;

		// Frame Payload and Encrypt
		memcpy(&lorabuffer[lorasize], buffer, size);
		lorasize += size;

		// TODO::Select channel (get channel settings)

		// Get Rx1, Rx2 Delay..

		// Set-up using current channel information
		Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
										currentSpradingFactor, currentCodingRate,
										LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
										true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

		// Check Time on Air..
		// airtime = Radio.TimeOnAir(lorabuffer, lorasize);

		// Tx to Radio
		Radio.Send( lorabuffer, lorasize );
	}
	else if (direction == RMSS_DN_LINK)
	{
		/*
		// TODO::Check joined network ..
		//if(MacStatus.Joined == LoRa_MAC_NotJoined)
		//{
		//	return;
		//}

		// Check current MAC status..
		// If MAC is busy (Tx running or Tx waiting) -> Cannot transmission..

		// Generate Frame Header
		mHeader.bits.Major = LORAWAN_R1;
		if(ack_req == 0)
		{
			//Unconfirmed Message
			mHeader.bits.MType = LoRa_Frame_Unconfirm_Data_Down;
		}
		else
		{
			//Confirmed Message
			mHeader.bits.MType = LoRa_Frame_Confirm_Data_Down;
		}

		lorabuffer[0] = mHeader.value;
		lorasize = 1;

		// Generate Frame Header.. DevAddr(4) FCtrl(1) FCnt(2) FOpts(0..15)
		fHeader.DevAddr.Bit.NwkAddr = Nwk_Addr;
		fHeader.DevAddr.Bit.NwkID = Nwk_ID;
		// ADR control
		if(MacStatus.ADR == LoRa_MAC_ADR_NETWORK)
			fHeader.FCtrl.dn.bits.ADR = LoRa_MAC_ADR_NETWORK;
		else
			fHeader.FCtrl.dn.bits.ADR = LoRa_MAC_ADR_DEVICE;

		if(MacStatus.ACKRequested == LoRa_MAC_ACK_Requested)
			fHeader.FCtrl.dn.bits.ACK = LoRa_MAC_ACK_Requested;
		else
			fHeader.FCtrl.dn.bits.ACK = LoRa_MAC_ACK_Not_Request;

		// Frame Pending Bit
		if(MacStatus.FPending == LoRa_MAC_PENDING)
			fHeader.FCtrl.dn.bits.ACK = LoRa_MAC_PENDING;
		else
			fHeader.FCtrl.dn.bits.ACK = LoRa_MAC_NO_PENDING;

		fHeader.FCtrl.dn.bits.FOptsLen = 0;

		fHeader.FrameCounter = FCntDown++;

		memcpy(&lorabuffer[lorasize], &fHeader.DevAddr.Address, 4);
		lorasize += 4;
		lorabuffer[lorasize]= fHeader.FCtrl.dn.value;
		lorasize += 1;
		memcpy(&lorabuffer[lorasize], &fHeader.FrameCounter, 2);
		lorasize += 2;

		// Frame Port
		lorabuffer[lorasize] = port;
		lorasize += 1;

		// Frame Payload and Encrypt
		memset(&lorabuffer[lorasize], '\0', size);
		EncryptPayload (&lorabuffer[lorasize], buffer, size, direction, fHeader, port, NwkSKey, AppSKey);
		//LoRaMacPayloadEncrypt( (uint8_t* ) payload, LoRaMacTxPayloadLen, LoRaMacAppSKey, LoRaMacDevAddr, UP_LINK, UpLinkCounter, &LoRaMacBuffer[pktHeaderLen] );
		lorasize += size;

		//TODO:: MIC .. using LoRa crypto
		//LoRaMacComputeMic( LoRaMacBuffer, LoRaMacBufferPktLen, LoRaMacNwkSKey, LoRaMacDevAddr, UP_LINK, UpLinkCounter, &mic );
		GenerateMIC (lorabuffer, lorasize, direction, fHeader, NwkSKey, mic);

		lorabuffer[lorasize++] = mic[0];
		lorabuffer[lorasize++] = mic[1];
		lorabuffer[lorasize++] = mic[2];
		lorabuffer[lorasize++] = mic[3];

		// TODO::Select channel (get channel settings)

		// Get Rx1, Rx2 Delay..

		// Set-up using current channel information
		Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
										currentSpradingFactor, currentCodingRate,
										LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
										true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

		// Check Time on Air..
		// airtime = Radio->TimeOnAir(lorabuffer, lorasize);

		// Tx to Radio
		Radio.Send( lorabuffer, lorasize );

		// Set MAC Status
		// OK?
		// Return Tx_OK;
		 */
	}
}

#ifdef LoRaGateWay
void RmssSendBeacon(RmssBcType_t bcType)
{
	uint8_t lorabuffer[256];
	uint8_t lorasize = 0;
	RmssMacBcHeader_t fHeader;

	// Up-Link (Device -> GW)
	if(bcType == RMSS_BC_NW_INITILIZATION)
	{
		memset(lorabuffer, 0 , 256);

		fHeader.BcFrameHeader.bcType = RMSS_BC_NW_INITILIZATION;
		fHeader.BcFrameHeader.groupIndex = 0;
		fHeader.BcFrameHeader.isContainSchedule = FALSE;

		memcpy(&lorabuffer[lorasize], &fHeader.BcFrameHeader, 1);
		lorasize += 1;	// sizeof(fHeader.BcFrameHeader);

		fHeader.gwAddress = DEV_ID;

		memcpy(&lorabuffer[lorasize], &fHeader.gwAddress, 2);
		lorasize += 2;

		fHeader.FrameCount.frameCnt = FALSE;
		fHeader.FrameCount.superFrameCnt = BcCounter;

		memcpy(&lorabuffer[lorasize], &fHeader.Fcnt, 2);
		lorasize += 2;

		BcCounter++;		// Increase beacon counter by 1

		// In the bootstrap phase, Beacon msg is sent on a fixed pre-defined channel
		// Using SF7 and CR1 which is specified in RmssMacInit()
		Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
										currentSpradingFactor, currentCodingRate,
										LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
										true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

		// Check Time on Air..
		// airtime = Radio.TimeOnAir(lorabuffer, lorasize);

		// Tx to Radio
		Radio.Send( lorabuffer, lorasize );
	}
	else if (bcType == RMSS_BC_START_FRAME)
	{
		/*
		// TODO::Check joined network ..
		//if(MacStatus.Joined == LoRa_MAC_NotJoined)
		//{
		//	return;
		//}

		// Check current MAC status..
		// If MAC is busy (Tx running or Tx waiting) -> Cannot transmission..

		// Generate Frame Header
		mHeader.bits.Major = LORAWAN_R1;
		if(ack_req == 0)
		{
			//Unconfirmed Message
			mHeader.bits.MType = LoRa_Frame_Unconfirm_Data_Down;
		}
		else
		{
			//Confirmed Message
			mHeader.bits.MType = LoRa_Frame_Confirm_Data_Down;
		}

		lorabuffer[0] = mHeader.value;
		lorasize = 1;

		// Generate Frame Header.. DevAddr(4) FCtrl(1) FCnt(2) FOpts(0..15)
		fHeader.DevAddr.Bit.NwkAddr = Nwk_Addr;
		fHeader.DevAddr.Bit.NwkID = Nwk_ID;
		// ADR control
		if(MacStatus.ADR == LoRa_MAC_ADR_NETWORK)
			fHeader.FCtrl.dn.bits.ADR = LoRa_MAC_ADR_NETWORK;
		else
			fHeader.FCtrl.dn.bits.ADR = LoRa_MAC_ADR_DEVICE;

		if(MacStatus.ACKRequested == LoRa_MAC_ACK_Requested)
			fHeader.FCtrl.dn.bits.ACK = LoRa_MAC_ACK_Requested;
		else
			fHeader.FCtrl.dn.bits.ACK = LoRa_MAC_ACK_Not_Request;

		// Frame Pending Bit
		if(MacStatus.FPending == LoRa_MAC_PENDING)
			fHeader.FCtrl.dn.bits.ACK = LoRa_MAC_PENDING;
		else
			fHeader.FCtrl.dn.bits.ACK = LoRa_MAC_NO_PENDING;

		fHeader.FCtrl.dn.bits.FOptsLen = 0;

		fHeader.FrameCounter = FCntDown++;

		memcpy(&lorabuffer[lorasize], &fHeader.DevAddr.Address, 4);
		lorasize += 4;
		lorabuffer[lorasize]= fHeader.FCtrl.dn.value;
		lorasize += 1;
		memcpy(&lorabuffer[lorasize], &fHeader.FrameCounter, 2);
		lorasize += 2;

		// Frame Port
		lorabuffer[lorasize] = port;
		lorasize += 1;

		// Frame Payload and Encrypt
		memset(&lorabuffer[lorasize], '\0', size);
		EncryptPayload (&lorabuffer[lorasize], buffer, size, direction, fHeader, port, NwkSKey, AppSKey);
		//LoRaMacPayloadEncrypt( (uint8_t* ) payload, LoRaMacTxPayloadLen, LoRaMacAppSKey, LoRaMacDevAddr, UP_LINK, UpLinkCounter, &LoRaMacBuffer[pktHeaderLen] );
		lorasize += size;

		//TODO:: MIC .. using LoRa crypto
		//LoRaMacComputeMic( LoRaMacBuffer, LoRaMacBufferPktLen, LoRaMacNwkSKey, LoRaMacDevAddr, UP_LINK, UpLinkCounter, &mic );
		GenerateMIC (lorabuffer, lorasize, direction, fHeader, NwkSKey, mic);

		lorabuffer[lorasize++] = mic[0];
		lorabuffer[lorasize++] = mic[1];
		lorabuffer[lorasize++] = mic[2];
		lorabuffer[lorasize++] = mic[3];

		// TODO::Select channel (get channel settings)

		// Get Rx1, Rx2 Delay..

		// Set-up using current channel information
		Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
										currentSpradingFactor, currentCodingRate,
										LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
										true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

		// Check Time on Air..
		// airtime = Radio->TimeOnAir(lorabuffer, lorasize);

		// Tx to Radio
		Radio.Send( lorabuffer, lorasize );

		// Set MAC Status
		// OK?
		// Return Tx_OK;
		 */
	}
}
#endif


/******************************************************************************
* Function Name        : ReceiveFrame
* Input Parameters     : None
* Return Value         : None
* Function Description : When LoRa MAC status change to RX,
*                      : LoRaMacStatusUpdate calls it.
*                      : It can handle both gateway and end device.
******************************************************************************/
void ReceiveFrame(void)
{
	RMSSMacHeader_t fHeader;		// LoRa Frame Header
	uint8_t len;
	uint8_t *payload;
	uint16_t size;
	uint8_t srcAddr[2];
//	int16_t rssi;
//	int8_t snr;
	//
//	uint8_t tx_information[128];

	payload = rxframe.payload;
	size = (uint16_t)rxframe.frameSize;

	// Get MAC Header..
	len = 0;
	memcpy(&fHeader.macHeader, &payload[len], 1);
	len++;

	// Find MType..
	// GW just receives and forwards to Network Server... Network Server has to handle LoRa Frame..
	switch(fHeader.macHeader.bits.MType)
	{
#if defined (LoRaGateWay)
	// GW: Join Request .. GW received Join Request Msg. from End Device ..
	case RMSS_Frame_Join_Request:

		// GW forward Join Request to Network Server
		rxframe.appPayloadSize = 0;
		rxframe.startOfAppPayload = 0;
		rxframe.endOfFrame = END_OF_FRAME;

		// Callback to the GW application..
		//RxHandlerGW(rxframe);

		break;
#endif

#if defined (LoRaEndDevice)
	// ED: Join Accept Response .. End Device received Join Accept Response Msg. from GW ..
	case RMSS_Frame_Join_Accept:
		// Check already joined?
		if (MacStatus.Joined == RMSS_MAC_Joined)
		{
			// Ignore Received Join Accept Msg.
			return;
		}
		break;
#endif

#if defined (LoRaGateWay)

	// GW: Unconfirmed Data Up .. GW received Unconfirmed Data from End Device ..
	// GW: Confirmed Data Up .. GW received Confirmed Data from End device ..
	case RMSS_Frame_Unconfirm_Data_Up:
	case RMSS_Frame_Confirm_Data_Up:
		// Get MAC Payload
		// Check the Dest. Address
		memcpy(&fHeader.DevAddr.Address, &payload[len], 2);
		len += 2;

		srcAddr[0] = (uint8_t)(fHeader.DevAddr.Address & 0xFF);
		srcAddr[1] = (uint8_t)((fHeader.DevAddr.Address >> 8) & 0xFF);

		memcpy(&fHeader.Fcnt, &payload[len], 2);
		len += 2;

		// Application Data

/*			rxframe.startOfFrame = START_OF_FRAME;
		rxframe.rssi = rssi;
		rxframe.snr = snr;
		// TODO:: How to Get this information when we using SX1301?
		rxframe.sf = currentSpradingFactor;				// How to get?
		rxframe.cr = currentCodingRate;					// How to get?
		rxframe.frameSize = size;*/
		rxframe.appPayloadSize = size - len;		// MIC length = 4
		rxframe.startOfAppPayload = len;
/*			memset(rxframe.payload, 0, sizeof(rxframe.payload));
		memcpy(rxframe.payload, payload, size);*/
		rxframe.endOfFrame = END_OF_FRAME;

		//RxHandlerGW(rxframe);

		sprintf(tx_information,"GW receives a packet from ED %03d\n", fHeader.Fcnt);
		tx_information[34] = 0;
		board_print((char*)tx_information);
		// Callback to the application..

		break;
#endif

#if defined(LoRaEndDevice)
	// ED: Unconfirmed Data Down .. End Device received Unconfirmed Data from GW ..
	// ED: Confirmed Data Down .. End Device received Confirmed Data from GW ..
	case RMSS_Frame_Unconfirm_Data_Down:
	case RMSS_Frame_Confirm_Data_Down:
		board_print("Node receives a packet from the GW\n");
		break;
#endif

	// Both: Proprietary ..
	case RMSS_Frame_Proprietary:
		break;

	// default ..
	default:
		// Unknown message type..
		break;

	}
	//State = RX;
}


/******************************************************************************
* Function Name        : LoRaMacStatusUpdate
* Input Parameters     : None
* Return Value         : None
* Function Description : Periodic status check call by main loop pooling
*                      : Only Class A is supported
******************************************************************************/
void LoraMacStatusUpdate(void)
{
	//LoRa MAC status change... -> Handling LoRa Frame here..

	switch( State )
	{
	case RX:
		// LoRa MAC received Frame from LoRa Interface
		ReceiveFrame();
		State = LOWPOWER;
		break;
	case TX:
		// Tx done..
    	State = RX_WAITING1;
#if defined(LoRaGateWay)
#else
    	Radio.Rx( RX_TIMEOUT_VALUE );
#endif
    	break;
	case RX_TIMEOUT_WAITING1:
#if defined (LoRaGateWay)
    	Radio.Rx( 0 );
#else
    	// Open one 2nd Rx slot
    	Radio.Rx( RX_TIMEOUT_VALUE );
#endif
		State = RX_WAITING2;
		break;
	case RX_TIMEOUT_WAITING2:
		// Close Rx time slot
		State = LOWPOWER;
		break;
    case RX_TIMEOUT:
    case RX_ERROR:
#if defined (LoRaGateWay)
    	Radio.Rx( 0 );
#else
    	Radio.Rx( RX_TIMEOUT_VALUE );
#endif
    	State = LOWPOWER;
    	break;
    case TX_TIMEOUT:
#if defined (LoRaGateWay)
    	Radio.Rx( 0 );
#else
    	Radio.Rx( RX_TIMEOUT_VALUE );
#endif
    	State = LOWPOWER;
		break;
	case LOWPOWER:
	case RX_WAITING1:
	case RX_WAITING2:
	default:
		break;
	}

}


/******************************************************************************
* Function Name        : IsNetworkJoined
* Input Parameters     : None
* Return Value         : 1 - End Device is joined LoRa network
*                      : 0 - End Device is not joined
* Function Description : Network Join status check..
******************************************************************************/
uint8_t IsNetworkJoined(void)
{
	if(MacStatus.Joined == RMSS_MAC_Joined)
		return 1;
	else
		return 0;
}

