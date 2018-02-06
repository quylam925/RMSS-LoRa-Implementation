/******************************************************************************
* Filename     : main.c                                                       *
* Program      : Private LoRa End Device Main Routine                         *
* Copyright    : Copyright (C) 2017-2017, ETRI                                *
*              : URL << http://www.etri.re.kr >>                              *
* Description  : Private LoRa End Device Application                          *
* Created at   : Mon Mar 27 2017.                                             *
* Authors      : Woo-Sung Jung (woosung@etri.re.kr)                           *
*              : Tae Hyun Yoon (thyoon0820@etri.re.kr)                        *
*              : Dae Seung Yoo (ooseyds@etri.re.kr)                           *
* Modified by  :                                                              *
* Modified at  :                                                              *
******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <main.h>
#include <stm32f1xx_hal.h>
#include <rtc.h>
#include <spi.h>
#include <usart.h>
#include <usb_device.h>
#include <gpio.h>
#include <radio.h>
#include <sx1276.h>
#include <sx1276-ubiboard.h>
#include <timer.h>
#include <delay.h>
#include <vcp.h>
#include <conf.h>
#include <application.h>
#include <RMSS_lora_mac.h>

//#include "cmsis_os.h"

/* Private variables ---------------------------------------------------------*/
//osThreadId networkConstructionTaskHandle;
//osThreadId testTaskHandle;

// defined in uart.c for UWB
extern uint8_t U1_RxBuffer[U1_RXBUFFERSIZE];
extern uint8_t u1_rx_uwb_eof;
extern uint8_t u1_rx_uwb_err;
extern uint8_t u1_rx_uwb_err_cnt;

// defined in loramac.c
extern uint8_t currentCodingRate;
extern uint8_t currentSpradingFactor;

// local variables
static bool SystemWakeupTimeCalibrated = false;
static TimerEvent_t CalibrateSystemWakeupTimeTimer;
#ifdef LoRaGateWay
static TimerEvent_t NetworkFormingTimer;		// This timer is used for sending periodic BC by the GW
#endif

// Statistics
uint16_t total_tx =0;
uint16_t total_rx =0;
static uint8_t tx_infor[256];

/* Private function prototypes -----------------------------------------------*/
void CalibrateSystemWakeupTime( void );
static void OnCalibrateSystemWakeupTimeTimerEvent( void );
void SystemClock_Config(void);
void Error_Handler(void);
static void MX_NVIC_Init(void);

void NetworkFormingTimerEventHandler( void );

// Call by loramac.c
void RxHandlerDevice(RMSSRxFrameInfo_t rxframe);
void RxHandlerGW(RMSSRxFrameInfo_t rxframe);

void NetworkConstructionTask(void const * argument);
void TestTask(void const * argument);

/******************************************************************************
* Function Name        : main
* Input Parameters     : None
* Return Value         : None
* Function Description : LoRa Interface Controller main routine
*                      : Infinite looping for communication
*                      : TODO: Power saving mechanism will be implemented
******************************************************************************/
int main(void)
{
	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();

	/* Test GPIO */
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
	DelayMs(300);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
	DelayMs(300);

	//MX_SPI1_Init();	// using USER_DEFINED INIT instead of SPI-auto generation code
	SpiInit( &SX1276.Spi, RADIO_MOSI, RADIO_MISO, RADIO_SCLK, NC );

	// communication with UWB or et.al.
	MX_USART1_Init();

	// Communication via USB
	// USB is disable because we dont connect the LoRa mote to Raspberry by USB protocol
	//MX_USB_DEVICE_Init();

	//MX_RTC_Init();	// using USER DEFINED INIT
	RtcInit();

	/* Initialize interrupts */
	MX_NVIC_Init();

	/* USER CODE BEGIN 2 */

    //SX1276IoInit( );
	RMSS_MAC_Init();

	CalibrateSystemWakeupTime( );

	// External LED ON
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

	/* Create the thread(s) */
	/* definition and creation of defaultTask */
	//osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
	//defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

	/* Start scheduler */
	//osKernelStart();

	//Application data initialization
	//board_print("After Kernel start\n");

#if defined (UBICOM_APP)
	UbicomApp_t app;
	ApplicationInitUbicom(&app);
#endif

#if defined(AUTOSTART)
	tx_enable = 1;
#endif

	// At the beginning of communication, LoRa End Device/GW can listen any LoRa Packet
	Radio.Rx(0);

	tick = 0;

#if defined (LoRaEndDevice)
	// Debug for connecting Serial Monitor..
	//DelayMs(10000);
	board_print("End device tries to join the network\n");
	while (IsNetworkJoined() != 1)
	{
		if((tick % 5000) == 4999)
		{
			tick = 0;
			// Send LoRa network join pkt;
			SendJoinRequest();
		}
		DelayMs(1);		// Every 1 millisecond.. wait
		tick++;

		LoraMacStatusUpdate();
	}
	tick = 0;
	board_print("End device while loop starts\n");
#else
	board_print("Gateway starts sending network info\n");
	TimerInit(&NetworkFormingTimer, NetworkFormingTimerEventHandler);
	TimerSetValue(&NetworkFormingTimer, 1000);
	TimerStart(&NetworkFormingTimer);
#endif

	/* Infinite loop -------------------------------------------------------------*/
	while (1)
	{
		DelayMs(SECOND);		// Every 1 millisecond.. wait
		//board_print("Flag raises\n");
	}// End of while loop
}

/* StartDefaultTask function */
void NetworkConstructionTask(void const * argument)
{
	static uint32_t time = 0;
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{
#ifdef DEBUG_ENABLED
		sprintf(tx_infor,"Time = %d\n", time);
		tx_infor[12] = 0;
		board_print((char*)tx_infor);
#endif
		//osDelay(SECOND);
		osDelay(SECOND);
		//DelayMs(SECOND);
		time++;
	}
	/* USER CODE END 5 */
}

void TestTask(void const * argument)
{
	static uint32_t time2 = 0;
	/* USER CODE BEGIN 5 */
	/* Infinite loop */
	for(;;)
	{
#ifdef DEBUG_ENABLED
		sprintf(tx_infor2,"TestTask Time= %d\n", time2);
		tx_infor2[20] = 0;
		board_print((char*)tx_infor2);
#endif

		osDelay(3*SECOND);
		//DelayMs(3*SECOND);
		time2++;
	}
	/* USER CODE END 5 */
}


/******************************************************************************
* Function Name        : RxHandlerDevice
* Input Parameters     : LoRaRxFrameInfo_t rxFrame - store Rx LoRa Frame
* Return Value         : None
* Function Description : End device handles Rx Pkt..
******************************************************************************/
void RxHandlerDevice(RMSSRxFrameInfo_t rxFrame)
{
	total_rx++;

	// Get information RSSI/SNR/CR/SF/FrameSize/ApplicationSize/startOfAppLocation/PAYLOAD
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);

	// Implementing here...
#if defined (RWSMS_ON)
	ApplicationRxHandlerRWSMS(&rxFrame.payload[rxFrame.startOfAppPayload]);
#elif defined (GDD_ON)

#else

#endif
}

/******************************************************************************
* Function Name        : RxHandlerGW
* Input Parameters     : LoRaRxFrameInfo_t rxframe - store Rx LoRa Frame
* Return Value         : None
* Function Description : GW forward Rx Pkt via VCP port..
*                      : TODO: It will be replace to SPI communication..
******************************************************************************/
void RxHandlerGW(RMSSRxFrameInfo_t rxframe)
{
	/*
	uint8_t temp[266];
	uint16_t len;
	uint16_t txCnt;
//	uint16_t result;

	total_rx++;
	payload = rxframe.payload;

	// Get information SOF/RSSI/SNR/CR/SF/FrameSize/ApplicationSize/startOfAppLocation/PAYLOAD.../EOF

	//Communication with GW forwarding program
	len = rxframe.frameSize;
	if(len > 266)
	{
		// Error occurs... we cannot handle it
		// VCP_write("2",1);
		return;
	}

	rxframe.payload[rxframe.frameSize] = rxframe.endOfFrame;

	// Copy date from Rx buffer to avoiding Rx buffer overwriting
	memcpy(temp, &rxframe, len);

	txCnt = 0;
	*/

}

#if defined (ADHOC_CONNECTION)
/******************************************************************************
* Function Name        : RxHandlerAdhoc
* Input Parameters     : LoRaRxFrameInfo_t rxframe - store Rx LoRa Frame
* Return Value         : None
* Function Description : Only Payload will be return via VCP
******************************************************************************/
void RxHandlerAdhoc(LoRaRxFrameInfo_t rxframe)
{
	uint8_t temp[272];
	uint16_t len;

	total_rx++;

	// Get information SOF/RSSI/SNR/CR/SF/FrameSize/ApplicationSize/startOfAppLocation/PAYLOAD.../EOF

	//Communication with GW forwarding program
	len = rxframe.frameSize + TUNNELING_OVERHEAD;
	if(len > 269)
	{
		// Error occurs... we cannot handle it
		return;
	}
	rxframe.payload[rxframe.frameSize] = rxframe.endOfFrame;

	// Copy date from Rx buffer to avoiding Rx buffer overwriting
	memcpy(temp, &rxframe, len);
	temp[len] = 0;

	// Send to Gateway Forwarding program
	VCP_write(&temp[18], rxframe.frameSize-13);
	//DelayMs( 1 );

}
#endif



/** System Clock Configuration
*/
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInit;

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.LSIState = RCC_LSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/**Initializes the CPU, AHB and APB busses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}

	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_USB;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV128;
	PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
	{
		Error_Handler();
	}

	/**Configure the Systick interrupt time
	*/
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/**Configure the Systick
	*/
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void HAL_SYSTICK_Callback(void){
	/* Implement my code here */

}
/** NVIC Configuration
*/
static void MX_NVIC_Init(void)
{
	/* RTC_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(RTC_IRQn, 4, 0);
	HAL_NVIC_EnableIRQ(RTC_IRQn);
	/* USART1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(USART1_IRQn, 8, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
	/* USB_LP_CAN1_RX0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	/* EXTI0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	/* EXTI1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	/* EXTI2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	/* EXTI3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	/* EXTI4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	/* EXTI9_5_IRQn interrupt configuration - [UBI] disable */
	//HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	//HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void CalibrateSystemWakeupTime( void )
{
    if( SystemWakeupTimeCalibrated == false )
    {
        TimerInit( &CalibrateSystemWakeupTimeTimer, OnCalibrateSystemWakeupTimeTimerEvent );
        TimerSetValue( &CalibrateSystemWakeupTimeTimer, 1000 );
        TimerStart( &CalibrateSystemWakeupTimeTimer );
        while( SystemWakeupTimeCalibrated == false )
        {
            TimerLowPowerHandler( );
        }
    }
}

void NetworkFormingTimerEventHandler( void )
{

	TimerReset( &MyTimer);
	myTimer += 1 ;
#ifdef DEBUG_ENABLED
		sprintf(tx_infor,"Time = %d\n", myTimer);
		tx_infor[14] = 0;
		board_print((char*)tx_infor);
#endif
}

static void OnCalibrateSystemWakeupTimeTimerEvent( void )
{
    SystemWakeupTimeCalibrated = true;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
	  DelayMs(1000);
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

