/**
  ******************************************************************************
  * @file    py32f0xx_it.c
  * @author  MCU Application Team
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "py32f0xx_it.h"

#include "main.h"
#include "config.h"


/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers         */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
}

/**
  * @brief This function handles System tick timer.
  */


//1ms SysTick
void SysTick_Handler(void)
{
	
	OsTimeTaskCB();
	 
}

/******************************************************************************/
/* PY32F0xx Peripheral Interrupt Handlers                                     */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_py32f003xx.s).                   */
/******************************************************************************/
//定时器1中断  10us  PWM控制
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	if((LL_TIM_ReadReg(TIM1,SR) & LL_TIM_SR_UIF) == 1 && LL_TIM_IsEnabledIT_UPDATE(TIM1))
	{
		LL_TIM_ClearFlag_UPDATE(TIM1);
		
		/////////////////////////////////////////////////////////////////////////////////////
		//Your codes
		
		/////////////////////////////////////////////////////////////////////////////////////
		

	}
}



//定时器16中断  20us  RF解码
void TIM16_IRQHandler(void)
{
    
	if((LL_TIM_ReadReg(TIM16,SR) & LL_TIM_SR_UIF) == 1 && LL_TIM_IsEnabledIT_UPDATE(TIM16))
	{
	
		LL_TIM_ClearFlag_UPDATE(TIM16);

		/////////////////////////////////////////////////////////////////////////////////////
		//Your codes
		
		/////////////////////////////////////////////////////////////////////////////////////

		
	}
}


//低功耗定时器中断
void LPTIM1_IRQHandler(void)
{
	   
	if(LL_LPTIM_IsActiveFlag_ARRM(LPTIM) == 1)
	{
		LL_LPTIM_ClearFLAG_ARRM(LPTIM);

		/////////////////////////////////////////////////////////////////////////////////////
		//Your codes
		
		/////////////////////////////////////////////////////////////////////////////////////

	}
}

	 
void EXTI0_1_IRQHandler(void)
{
	if(LL_EXTI_ReadFlag(LL_EXTI_LINE_0) == LL_EXTI_LINE_0)
    {
        LL_EXTI_ClearFlag(LL_EXTI_LINE_0);
		/////////////////////////////////////////////////////////////////////////////////////
		//Your codes
		
		/////////////////////////////////////////////////////////////////////////////////////
    }
}

void EXTI4_15_IRQHandler(void)
{
	if(LL_EXTI_ReadFlag(LL_EXTI_LINE_7) == LL_EXTI_LINE_7)
    {
        LL_EXTI_ClearFlag(LL_EXTI_LINE_7);
		/////////////////////////////////////////////////////////////////////////////////////
		//Your codes
		
		/////////////////////////////////////////////////////////////////////////////////////
    }
}


/************************ (C) COPYRIGHT Puya *****END OF FILE****/
