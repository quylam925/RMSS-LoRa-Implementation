/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include <stdlib.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
#define RADIO_RESET                                 PB_7

#define RADIO_MOSI                                  PA_7
#define RADIO_MISO                                  PA_6
#define RADIO_SCLK                                  PA_5
#define RADIO_NSS                                   PA_4

#define RADIO_DIO_0                                 PB_0
#define RADIO_DIO_1                                 PB_1
#define RADIO_DIO_2                                 PB_2
#define RADIO_DIO_3                                 PB_3
#define RADIO_DIO_4                                 PB_4
#define RADIO_DIO_5                                 PB_5

#define RADIO_ANT_SWITCH_HF                         PA_1
#define RADIO_ANT_SWITCH_LF                         PA_0

//#define OSC_LSE_IN                                  PC_14
//#define OSC_LSE_OUT                                 PC_15

#define OSC_HSE_IN                                  PD_0
#define OSC_HSE_OUT                                 PD_1

#define USB_DM                                      PA_11
#define USB_DP                                      PA_12

//#define I2C_SCL                                     PB_6
//#define I2C_SDA                                     PB_7

//#define BOOT_1                                      PB_2

//#define GPS_PPS                                     PB_1
#define UART_TX                                     PA_9
#define UART_RX                                     PA_10

//#define DC_DC_EN                                    PB_8
//#define BAT_LEVEL                                   PB_0
//#define WKUP1                                       PA_8
//#define USB_ON                                      PA_2

#define RF_RXTX                                     PB_9

#define SWDIO                                       PA_13
#define SWCLK                                       PA_14

//#define TEST_POINT1                                 PB_12
//#define TEST_POINT2                                 PB_13
//#define TEST_POINT3                                 PB_14
//#define TEST_POINT4                                 PB_15

//#define PIN_NC                                      PB_5

//#define LED_PIN										GPIO_PIN_15

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
