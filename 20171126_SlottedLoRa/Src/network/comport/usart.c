/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "gpio.h"
#include "delay.h"
#include "conf.h"
#include "sx1276.h"
#include <sx1276-ubiboard.h>

/* USER CODE BEGIN 0 */
uint16_t u1_rx_cnt = 0;					// u1_rx_cnt represents Rx count (Bytes) in the USART Interrupt handler
//uint8_t uart1_rx_done = 0;					// u1_rx_done for UWB protocol
uint8_t U1_RxBuffer[U1_RXBUFFERSIZE];   	// USART1 Rx buffer
uint8_t u1_rx_uwb_sof;
uint8_t u1_rx_uwb_eof;
uint8_t u1_rx_uwb_err;
uint8_t u1_rx_uwb_err_cnt;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_Init(void)
{

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	// For Uart Rx Interrupt for UWB module
	u1_rx_uwb_sof = 0;
	u1_rx_uwb_eof = 0;
	u1_rx_uwb_err = 0;
	u1_rx_uwb_err_cnt = 0;
}
#if 1
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if(uartHandle->Instance==USART1)
	{
	/* USER CODE BEGIN USART1_MspInit 0 */

	/* USER CODE END USART1_MspInit 0 */
	/* Peripheral clock enable */
	__HAL_RCC_USART1_CLK_ENABLE();

	/**USART1 GPIO Configuration
	PA9     ------> USART1_TX
	PA10     ------> USART1_RX
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* Peripheral interrupt Deinit*/
    HAL_NVIC_DisableIRQ(USART1_IRQn);

  }
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
} 
#endif
/* USER CODE BEGIN 1 */

// Receive from UART then send to LoRa Radio.
uint16_t Uart1_ReceiveData(uint8_t * Data){
	uint16_t size;
//	if(u1_rx_cnt > 0){
		// Delay 1s for 9600 bps device
		//HAL_Delay(1000);

		//VCP_write(U1_RxBuffer,u1_rx_cnt);						// Send UART1 Rx data to Virtual COM Port

		//TODO: Do we need 0 filling?
		for(uint8_t i = u1_rx_cnt; i < U1_RXBUFFERSIZE; i++ ) {		// BUFFER_SIZE : Radio Max Size
			U1_RxBuffer[i] = 0;
		}

		DelayMs( 1 );
		//Radio.Send( U1_RxBuffer, BUFFER_SIZE );
		size = u1_rx_cnt;
		Radio.Send( U1_RxBuffer, u1_rx_cnt );

		for(uint8_t i = 0 ; i < u1_rx_cnt; i++){
			Data[i] = U1_RxBuffer[i];
		}

		u1_rx_cnt = 0;
		return size;
//	}

	return u1_rx_cnt;
}

void UWB_to_LoRa (void) {
	//for prevent buffer overflow
	// TODO:: When LoRa Pkt size increase, cnt upper bound will be increased
	if(u1_rx_cnt > 64)
		u1_rx_cnt = 64;

	// Do we need 0 filling?
	Radio.Send( U1_RxBuffer, u1_rx_cnt );
	u1_rx_uwb_sof = 0;
	u1_rx_uwb_eof = 0;
	u1_rx_cnt = 0;
}

void Uart1_SendData(UART_HandleTypeDef *huart, uint8_t* Buffer, uint16_t Size, uint8_t start){
	//Original
	for(uint8_t i = start ; i < Size ; i++){
		while(!(huart -> Instance -> SR & UART_FLAG_TXE));
		huart -> Instance -> DR = Buffer[i];
	}
	//
	/*
	for(uint8_t i = start ; i < Size ; i++){
		while(!(huart -> Instance -> SR & UART_FLAG_TXE));
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_15);
		huart -> Instance -> DR = Buffer[i];
	}
	*/
}


/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
