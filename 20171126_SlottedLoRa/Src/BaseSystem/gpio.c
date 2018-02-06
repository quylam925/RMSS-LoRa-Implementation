/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA0 PA1 PA15 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB1 PB2 PB3
						   PB4 PB5 */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
						  |GPIO_PIN_4|GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PB7 PB8 PB9 */
	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 2 */

static GpioIrqHandler *GpioIrq[16];

void GpioMcuInit( Gpio_t *obj, PinNames pin, PinModes mode, PinConfigs config, PinTypes type, uint32_t value )
{
    GPIO_InitTypeDef GPIO_InitStructure;

    if( pin == NC )
    {
        return;
    }
    obj->pin = pin;
    obj->pinIndex = ( 0x01 << ( obj->pin & 0x0F ) );

    if( ( obj->pin & 0xF0 ) == 0x00 )
    {
        obj->port = GPIOA;
        __HAL_RCC_GPIOA_CLK_ENABLE( );
    }
    else if( ( obj->pin & 0xF0 ) == 0x10 )
    {
        obj->port = GPIOB;
        __HAL_RCC_GPIOB_CLK_ENABLE( );
    }
    else if( ( obj->pin & 0xF0 ) == 0x20 )
    {
        obj->port = GPIOC;
        __HAL_RCC_GPIOC_CLK_ENABLE( );
    }
    else if( ( obj->pin & 0xF0 ) == 0x30 )
    {
        obj->port = GPIOD;
        __HAL_RCC_GPIOD_CLK_ENABLE( );
    }
    else
    {

    }

    GPIO_InitStructure.Pin =  obj->pinIndex ;
    GPIO_InitStructure.Pull = type;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

    if( mode == PIN_INPUT )
    {
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    }
    else if( mode == PIN_ANALOGIC )
    {
        GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    }
    else if( mode == PIN_ALTERNATE_FCT )
    {
        if( config == PIN_OPEN_DRAIN )
        {
            GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
        }
        else
        {
            GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        }
        //GPIO_InitStructure.Alternate = value;
    }
    else // mode ouptut
    {
        if( config == PIN_OPEN_DRAIN )
        {
            GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
        }
        else
        {
            GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        }
    }

    HAL_GPIO_Init( obj->port, &GPIO_InitStructure );

    // Sets initial output value
    if( mode == PIN_OUTPUT )
    {
        GpioMcuWrite( obj, value );
    }
}

void GpioMcuSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
    uint32_t priority = 0;

    IRQn_Type IRQnb = EXTI0_IRQn;
    GPIO_InitTypeDef   GPIO_InitStructure;

    if( irqHandler == NULL )
    {
        return;
    }

    GPIO_InitStructure.Pin =  obj->pinIndex;

    if( irqMode == IRQ_RISING_EDGE )
    {
        GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    }
    else if( irqMode == IRQ_FALLING_EDGE )
    {
        GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
    }
    else
    {
        GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
    }

    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init( obj->port, &GPIO_InitStructure );

    switch( irqPriority )
    {
    case IRQ_VERY_LOW_PRIORITY:
    case IRQ_LOW_PRIORITY:
        priority = 3;
        break;
    case IRQ_MEDIUM_PRIORITY:
        priority = 2;
        break;
    case IRQ_HIGH_PRIORITY:
        priority = 1;
        break;
    case IRQ_VERY_HIGH_PRIORITY:
    default:
        priority = 0;
        break;
    }

    switch( obj->pinIndex )
    {
    case GPIO_PIN_0:
        IRQnb = EXTI0_IRQn;
        break;
    case GPIO_PIN_1:
        IRQnb = EXTI1_IRQn;
        break;
    case GPIO_PIN_2:
        IRQnb = EXTI2_IRQn;
        break;
    case GPIO_PIN_3:
        IRQnb = EXTI3_IRQn;
        break;
    case GPIO_PIN_4:
        IRQnb = EXTI4_IRQn;
        break;
    case GPIO_PIN_5:
    case GPIO_PIN_6:
    case GPIO_PIN_7:
    case GPIO_PIN_8:
    case GPIO_PIN_9:
        //IRQnb = EXTI9_5_IRQn;
        break;
    case GPIO_PIN_10:
    case GPIO_PIN_11:
    case GPIO_PIN_12:
    case GPIO_PIN_13:
    case GPIO_PIN_14:
    case GPIO_PIN_15:
        //IRQnb = EXTI15_10_IRQn;
        break;
    default:
        break;
    }

    GpioIrq[(obj->pin ) & 0x0F] = irqHandler;

    HAL_NVIC_SetPriority( IRQnb , priority, 0 );
    HAL_NVIC_EnableIRQ( IRQnb );
}

void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
    GPIO_InitTypeDef   GPIO_InitStructure;

    GPIO_InitStructure.Pin =  obj->pinIndex ;
    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init( obj->port, &GPIO_InitStructure );
}

void GpioMcuWrite( Gpio_t *obj, uint32_t value )
{
    if( ( obj == NULL ) || ( obj->port == NULL ) )
    {
        assert_param( FAIL );
    }
    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return;
    }
    HAL_GPIO_WritePin( obj->port, obj->pinIndex , ( GPIO_PinState )value );
}

void GpioMcuToggle( Gpio_t *obj )
{
    if( ( obj == NULL ) || ( obj->port == NULL ) )
    {
        assert_param( FAIL );
    }

    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return;
    }
    HAL_GPIO_TogglePin( obj->port, obj->pinIndex );
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
    if( obj == NULL )
    {
        assert_param( FAIL );
    }
    // Check if pin is not connected
    if( obj->pin == NC )
    {
        return 0;
    }
    return HAL_GPIO_ReadPin( obj->port, obj->pinIndex );
}

void EXTI0_IRQHandler( void )
{
#if !defined( USE_NO_TIMER )
    //RtcRecoverMcuStatus( );
#endif
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_0 );
}

void EXTI1_IRQHandler( void )
{
#if !defined( USE_NO_TIMER )
    //RtcRecoverMcuStatus( );
#endif
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_1 );
}

void EXTI2_IRQHandler( void )
{
#if !defined( USE_NO_TIMER )
    //RtcRecoverMcuStatus( );
#endif
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_2 );
}

void EXTI3_IRQHandler( void )
{
#if !defined( USE_NO_TIMER )
    //RtcRecoverMcuStatus( );
#endif
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_3 );
}

void EXTI4_IRQHandler( void )
{
#if !defined( USE_NO_TIMER )
    //RtcRecoverMcuStatus( );
#endif
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_4 );
}

void EXTI9_5_IRQHandler( void )
{
#if !defined( USE_NO_TIMER )
    //RtcRecoverMcuStatus( );
#endif
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_5 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_6 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_7 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_8 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_9 );
}

void EXTI15_10_IRQHandler( void )
{
#if !defined( USE_NO_TIMER )
    //RtcRecoverMcuStatus( );
#endif
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_10 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_11 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_12 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_13 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_14 );
    //HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_15 );
}

void HAL_GPIO_EXTI_Callback( uint16_t gpioPin )
{
    uint8_t callbackIndex = 0;

    if( gpioPin > 0 )
    {
        while( gpioPin != 0x01 )
        {
            gpioPin = gpioPin >> 1;
            callbackIndex++;
        }
    }

    if( GpioIrq[callbackIndex] != NULL )
    {
        GpioIrq[callbackIndex]( );
    }
}


void GpioInit( Gpio_t *obj, PinNames pin, PinModes mode,  PinConfigs config, PinTypes type, uint32_t value )
{
    if( ( uint32_t )( pin >> 4 ) <= 6 )
    {
        GpioMcuInit( obj, pin, mode, config, type, value );
    }
    else
    {
#if defined( BOARD_IOE_EXT )
        // IOExt Pin
        GpioIoeInit( obj, pin, mode, config, type, value );
#endif
    }
}

void GpioSetInterrupt( Gpio_t *obj, IrqModes irqMode, IrqPriorities irqPriority, GpioIrqHandler *irqHandler )
{
    if( ( uint32_t )( obj->pin >> 4 ) <= 6 )
    {
        GpioMcuSetInterrupt( obj, irqMode, irqPriority, irqHandler );
    }
    else
    {
#if defined( BOARD_IOE_EXT )
        // IOExt Pin
        GpioIoeSetInterrupt( obj, irqMode, irqPriority, irqHandler );
#endif
    }
}

void GpioRemoveInterrupt( Gpio_t *obj )
{
    if( ( uint32_t )( obj->pin >> 4 ) <= 6 )
    {
        //GpioMcuRemoveInterrupt( obj );
    }
    else
    {
#if defined( BOARD_IOE_EXT )
        // IOExt Pin
        //GpioIoeRemoveInterrupt( obj );
#endif
    }
}

void GpioWrite( Gpio_t *obj, uint32_t value )
{
    if( ( uint32_t )( obj->pin >> 4 ) <= 6 )
    {
        GpioMcuWrite( obj, value );
    }
    else
    {
#if defined( BOARD_IOE_EXT )
        // IOExt Pin
        GpioIoeWrite( obj, value );
#endif
    }
}

void GpioToggle( Gpio_t *obj )
{
    if( ( uint32_t )( obj->pin >> 4 ) <= 6 )
    {
        GpioMcuToggle( obj );
    }
    else
    {
#if defined( BOARD_IOE_EXT )
        // IOExt Pin
        GpioIoeWrite( obj, GpioIoeRead( obj ) ^ 1 );
#endif
    }
}

uint32_t GpioRead( Gpio_t *obj )
{
    if( ( uint32_t )( obj->pin >> 4 ) <= 6 )
    {
        return GpioMcuRead( obj );
    }
    else
    {
#if defined( BOARD_IOE_EXT )
        // IOExt Pin
        return GpioIoeRead( obj );
#else
        return 0;
#endif
    }
}



/* USER CODE END 2 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
