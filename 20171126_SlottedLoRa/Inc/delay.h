/**
  ******************************************************************************
  * File Name          : delay.h
  * Description        : This file contains the delay related headers of the application
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DELAY_H
#define __DELAY_H
  /* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/*!
 * Blocking delay of "s" seconds
 */
void Delay( float s );

/*!
 * Blocking delay of "ms" milliseconds
 */
void DelayMs( uint32_t ms );
/* USER CODE END Private defines */


#endif /* __DELAY_H */
