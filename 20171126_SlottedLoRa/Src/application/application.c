/******************************************************************************
* Filename     : application.c                                                *
* Program      : Private LoRa Application Handling Routine                    *
* Copyright    : Copyright (C) 2017-2017, ETRI                                *
*              : URL << http://www.etri.re.kr >>                              *
* Authors      : Woo-Sung Jung (woosung@etri.re.kr)                           *
*              : Tae Hyun Yoon (thyoon0820@etri.re.kr)                        *
*              : Dae Seung Yoo (ooseyds@etri.re.kr)                           *
* Description  : Private LoRa End Device Application                          *
* Created at   : Mon July 24 2017.                                            *
* Modified by  :                                                              *
* Modified at  :                                                              *
******************************************************************************/
#include <application.h>
#include <conf.h>
#include <rtc.h>
#include <vcp.h>
#include <string.h>

extern uint8_t currentCodingRate;
extern uint8_t currentSpradingFactor;
uint8_t AppKey[16];


/******************************************************************************
* Function Name        : ApplicationInit XXX
* Input Parameters     : Each Application Pointer
* Return Value         : None
* Function Description : Application Value Initialization..
******************************************************************************/

#if defined (UBICOM_APP)
void ApplicationInitUbicom(UbicomApp_t *app)
{
	uint8_t loopcnt;
	UbicomApp_t *appUbicom;
	appUbicom = (UbicomApp_t *)app;

	appUbicom->appID = UBICOM_APP_TYPE;
	appUbicom->msgID = DATA_MSG;

	for(loopcnt = 0; loopcnt < APP_PAYLOAD_LENGTH; loopcnt++)
		appUbicom->data[loopcnt] = 0;
}
#elif defined (RWSMS_ON)
void ApplicationInitRWSMS(RWSMS_t *app)
{
	uint8_t loopcnt;
	RWSMS_t *appRWSMS;
	appRWSMS = (RWSMS_t *)app;
	appRWSMS->seqNum = 0;
	appRWSMS->msgID = DATA_MSG;
	appRWSMS->pppID = DEV_ID&0xff;
	appRWSMS->wsID = 1;
	appRWSMS->ppeData[0] = 0;
	appRWSMS->ppeData[1] = 0;
	appRWSMS->ppeData[2] = 0;
	appRWSMS->ppeData[3] = 0;
	appRWSMS->ppeData[4] = 0;

	for(loopcnt = 0; loopcnt < 16; loopcnt++)
		AppKey[loopcnt] = 0;
}
#elif defined (GDD_ON)
void ApplicationInitGDD(GDD_t *app)
{
	uint8_t loopcnt;
	GDD_t *appGDD;
	appGDD = (GDD_t *)app;
	appGDD->oxygen = 22;
	appGDD->carbonOxygen = 1;
	appGDD->hydrogen = 2;
	appGDD->hydrogenSulfide = 1;
	appGDD->methane = 0;

	for(loopcnt = 0; loopcnt < 16; loopcnt++)
		AppKey[loopcnt] = 0;
}
#else


void ApplicationInitTemp(App_t	*app)
{
	uint8_t loopcnt;

	//Init APP
	app->ID = DEV_ID;
	app->SeqNum = 0;
	app->TxPwr = TX_OUTPUT_POWER;
	app->SF = currentSpradingFactor;
	app->CR = currentCodingRate;
	app->Time = RtcGetTimerValue();
	app->Time2 = 0;
	for(loopcnt = 0; loopcnt < 10; loopcnt++)
		app->pading[loopcnt] = '0'+loopcnt;
	for(loopcnt=10;loopcnt<36;loopcnt++)
		app->pading[loopcnt] = 'A'+loopcnt-10;
	for(loopcnt=36;loopcnt<48;loopcnt++)
		app->pading[loopcnt] = 'a'+loopcnt-36;

	for(loopcnt = 0; loopcnt < 16; loopcnt++)
		AppKey[loopcnt] = 0;

}
#endif

/******************************************************************************
* Function Name        : GetAppKey
* Input Parameters     : Nonce
* Return Value         : Application Key Value
* Function Description : Application Key Value return
******************************************************************************/
void GetAppKey(uint8_t *key)
{
	memcpy(key, AppKey, 16);
}

/******************************************************************************
* Function Name        : ApplicationGeneration XXX
* Input Parameters     : Each Application Pointer & return buffer
* Return Value         : None
* Function Description : Application Value Generation
******************************************************************************/
#if defined (UBICOM_APP)
void ApplicationGenerationUbicom(UbicomApp_t *app, uint8_t *tempbuffer)
{
	// Start of Frame
	tempbuffer[0] = 'u';
	tempbuffer[1] = 'b';
	tempbuffer[2] = 'i';
	// App ID
	tempbuffer[3] = app->appID + '0';

	// Msg ID
	tempbuffer[4] = app->msgID + '0';

	// Sequence Number
	tempbuffer[5] = ((app->seqNum/10) % 10) + '0';
	tempbuffer[6] = (app->seqNum % 10) + '0';

	for (int i = 7; i< APP_PAYLOAD_LENGTH; i++)
	{
		tempbuffer[i] = '7';
	}
	// End of String
	tempbuffer[APP_PAYLOAD_LENGTH] = 0;
}

#elif defined (RWSMS_ON)
void ApplicationGenerationRWSMS(RWSMS_t *app, uint8_t *tempbuffer)
{
	// Start of Frame
	tempbuffer[0] = 'm';
	tempbuffer[1] = 'i';
	tempbuffer[2] = 'c';

	// Sequence Number
	tempbuffer[3] = ((app->seqNum/10) % 10) + '0';
	tempbuffer[4] = (app->seqNum % 10) + '0';

	// Msg ID
	tempbuffer[5] = app->msgID + '0';

	// PPP ID
	tempbuffer[6] = ((app->pppID/100) % 10) + '0';
	tempbuffer[7] = ((app->pppID/10) % 10) + '0';
	tempbuffer[8] = (app->pppID % 10) + '0';

	// wsID
	tempbuffer[9] = app->wsID + '0';

	// PPE Data
	tempbuffer[10] = app->ppeData[0] + '0';
	tempbuffer[11] = app->ppeData[1] + '0';
	tempbuffer[12] = app->ppeData[2] + '0';
	tempbuffer[13] = app->ppeData[3] + '0';
	tempbuffer[14] = app->ppeData[4] + '0';

	// Reserved..
	tempbuffer[15] = '0';
	tempbuffer[16] = '0';

	// End of String
	tempbuffer[17] = 'f';
	tempbuffer[18] = 'i';
	tempbuffer[19] = 'n';
	tempbuffer[20] = 0;

	// For Length Test.. 242
/*	for (int i = 20; i< 242; i++)
	{
		tempbuffer[i] = '0';
	}
	tempbuffer[242] = 0;
*/
}

#elif defined (GDD_ON)
void ApplicationGenerationGDD(GDD_t *app, uint8_t *tempbuffer)
{
	sprintf(tempbuffer,"[O2:%d.0|CO:%d.0|H2:%d.0|H2S:%d.0|CH4:%d.0]\0",
				app->oxygen, app->carbonOxygen, app->hydrogen, app->hydrogenSulfide, app->methane);
}

#else
void ApplicationGenerationTemp(App_t	*app)
{
	uint8_t i;

	//Init APP
	app->ID = DEV_ID;
	//app->SeqNum = 0;
	app->TxPwr = TX_OUTPUT_POWER;
	app->SF = currentSpradingFactor;
	app->CR = currentCodingRate;
	app->Time = RtcGetTimerValue();
	app->Time2 = 0;
	for(i=0;i<10;i++)
		app->pading[i] = '0'+i;
	for(i=10;i<36;i++)
		app->pading[i] = 'A'+i-10;
	for(i=36;i<48;i++)
		app->pading[i] = 'a'+i-36;

}
#endif


/******************************************************************************
* Function Name        : ApplicationRxHandler XXX
* Input Parameters     : Received Payload Data
* Return Value         : None
* Function Description : Received Application Data Handler
******************************************************************************/
#if defined (RWSMS_ON)
void ApplicationRxHandlerRWSMS(uint8_t *rxPayload)
{
	uint8_t loopCnt;
	RWSMS_t app;
	char vcpMsg[64];

	memset(vcpMsg, 0, sizeof(vcpMsg));

	// Check Start of String
	if((rxPayload[0] != 'm') || (rxPayload[1] != 'i') || (rxPayload[2] != 'c'))
	{
		VCP_write("SOS Error\n",10);
		return;
	}

	// Get Sequence Number
	app.seqNum = (rxPayload[3] - '0') * 10 + (rxPayload[4] - '0');

	// IDs (msg, PPP, ws)
	app.msgID = (rxPayload[5] - '0');
	app.pppID = (rxPayload[6] - '0') * 100 + (rxPayload[7] - '0') * 10 + (rxPayload[8] - '0');
	app.wsID = (rxPayload[9] - '0');

	switch(app.msgID)
	{
	case RULE_MSG:
		for(loopCnt = 0; loopCnt < 5; loopCnt++)
		{
			app.ppeRule[loopCnt] = (rxPayload[10+loopCnt] - '0');
		}
		sprintf(vcpMsg, "Rule Data = %d, %d, %d, %d, %d\n",
						app.ppeRule[0], app.ppeRule[1], app.ppeRule[2], app.ppeRule[3], app.ppeRule[4]);
		VCP_write(vcpMsg, strlen(vcpMsg));
		break;
	case INTERVAL_MSG:
			app.interval = (rxPayload[10] - '0') * 10000 + (rxPayload[11] - '0') * 1000
						   + (rxPayload[12] - '0') * 100 + (rxPayload[13] - '0') * 10
						   + (rxPayload[14] - '0') ;
			sprintf(vcpMsg, "Msg. Interval = %ld\n", app.interval);
			VCP_write(vcpMsg, strlen(vcpMsg));
		break;
	case LED_BUZZER_MSG:
			app.led = (rxPayload[10] - '0');
			app.buzzer = (rxPayload[11] - '0');
			sprintf(vcpMsg, "LEB = %d, Buzzer = %d\n", app.led, app.buzzer);
			VCP_write(vcpMsg, strlen(vcpMsg));
		break;
	default:
		sprintf(vcpMsg, "We cannot handle this msg\n");
		VCP_write(vcpMsg, strlen(vcpMsg));
		break;
	}

	// End of String
	if((rxPayload[17] != 'm') || (rxPayload[18] != 'i') || (rxPayload[19] != 'c'))
	{
		VCP_write("EOS Error\n",10);
		return;
	}
}


#elif defined (GDD_ON)


#else



#endif




