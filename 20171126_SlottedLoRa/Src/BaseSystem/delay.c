/**
  ******************************************************************************
  * File Name          : delay.c
  * Description        : This file provides code for the delay HAL codes
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "delay.h"


/* User Codes -----------------------------------------------------------------*/

/**
 * @brief DelayMs waits time via HAL_Delay driver
 * @patam ms : waiting delay time (unit: milliseconds)
 * @retval   : none
*/
void DelayMs( uint32_t ms )
{
    HAL_Delay( ms );
}

/**
 * @brief Delay waits time via HAL_Delay driver
 * @patam s : waiting delay time (unit: seconds)
 * @retval  : none
*/
void Delay( float s )
{
    DelayMs( s * 1000.0f );
}


