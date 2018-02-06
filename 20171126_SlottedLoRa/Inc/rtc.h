/**
  ******************************************************************************
  * File Name          : RTC.h
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
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
#ifndef __rtc_H
#define __rtc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "main.h"
//#include "timer.h"

 /* USER CODE BEGIN Includes */

 /* USER CODE END Includes */

 extern RTC_HandleTypeDef hrtc;

 /* USER CODE BEGIN Private defines */

 /* USER CODE END Private defines */

 extern void Error_Handler(void);


 #ifndef TimerTime_t
 typedef uint32_t TimerTime_t;
 #endif

 /*!
  * \brief Initializes the RTC timer
  *
  * \remark The timer is based on the RTC
  */
 void RtcInit( void );

 /*!
  * \brief Start the RTC timer
  *
  * \remark The timer is based on the RTC Alarm running at 32.768KHz
  *
  * \param[IN] timeout Duration of the Timer
  */
 void RtcSetTimeout( uint32_t timeout );

 /*!
  * \brief Adjust the value of the timeout to handle wakeup time from Alarm and GPIO irq
  *
  * \param[IN] timeout Duration of the Timer without compensation for wakeup time
  * \retval new value for the Timeout with compensations
  */
 TimerTime_t RtcGetAdjustedTimeoutValue( uint32_t timeout );

 /*!
  * \brief Get the RTC timer value
  *
  * \retval RTC Timer value
  */
 TimerTime_t RtcGetTimerValue( void );

 /*!
  * \brief Get the RTC timer elapsed time since the last Alarm was set
  *
  * \retval RTC Elapsed time since the last alarm
  */
 TimerTime_t RtcGetElapsedAlarmTime( void );

 /*!
  * \brief Compute the timeout time of a future event in time
  *
  * \param[IN] futureEventInTime Value in time
  * \retval time Time between now and the futureEventInTime
  */
 TimerTime_t RtcComputeFutureEventTime( TimerTime_t futureEventInTime );

 /*!
  * \brief Compute the elapsed time since a fix event in time
  *
  * \param[IN] eventInTime Value in time
  * \retval elapsed Time since the eventInTime
  */
 TimerTime_t RtcComputeElapsedTime( TimerTime_t eventInTime );

 /*!
  * \brief This function blocks the MCU from going into Low Power mode
  *
  * \param [IN] status [true: Enable, false: Disable
  */
 void BlockLowPowerDuringTask ( bool status );

 /*!
  * \brief Sets the MCU into low power STOP mode
  */
 void RtcEnterLowPowerStopMode( void );

 /*!
  * \brief Restore the MCU to its normal operation mode
  */
 void RtcRecoverMcuStatus( void );



#ifdef __cplusplus
}
#endif
#endif /*__ rtc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
