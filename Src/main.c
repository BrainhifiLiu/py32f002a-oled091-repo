/**
 * Project Frame:      
 * File:         main.c
 * MCU:			 PY32F002AW15S - SOP16      FLASH 20K/ SRAM 3K
 * Description:
 *				SW-101-PYB 开发板
 * Author:        Tony Liu 
 * Version:       V1.00
 * Date:          2023.07.02
 --------------------------------------------------------------------------
 */

////////////////////////////////////////////////////////////////////////////////

/* Includes ------------------------------------------------------------------*/
#include "string.h"

#include "main.h"
//#include "py32f0xx_ll_iwdg.h"


#include "oled091.h" 

/* Private define ------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/



/* Private variables ---------------------------------------------------------*/
volatile uint16_t 	g_cnt_Timer_T1ms = 0;

uint8_t	b_update = 0;

uint8_t cnt_sec = 0;
uint8_t cnt_min = 0;
uint8_t cnt_hour = 0;

uint16_t g_cnt_Timer_Led = 0;

uint16_t  cnt_1sec = 1000;

int main(void)
{
    char info[20];

	
	memset(info,0,sizeof(info));


	
	//初始化系统
	APP_SystemInit();
	
	O_LED_ON;
	
    OLED_Init();
    OLED_Clear(); 

    OLED_ShowString(30,1,(uint8_t *)"Wellcome",Oled_Font_8x16);	
	
	
	delay_ms(1000);
	OLED_Clear();
	O_LED_OFF;
	
    OLED_ShowString(0,2,(uint8_t*)"---- --",Oled_Font_8x16);
    
	
	
	while (1)
	{

	
		if(b_update)
		{
			b_update = 0;
			sprintf(info,"%02d %02d:%02d",cnt_hour,cnt_min,cnt_sec); 
			OLED_ShowString(63,0,(uint8_t*)info,Oled_Font_8x8);						
		}
		
		 
	}
	
	 
}


void APP_SystemInit(void)
{
	/* 配置系统时钟 */
	APP_SystemClockConfig(); 
	
	APP_IOPortInit();	

	

	/*配置并开启TIM1计数模式*/
	APP_ConfigTIM1();
	

}	
	
void APP_IOPortInit(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA | LL_IOP_GRP1_PERIPH_GPIOB | LL_IOP_GRP1_PERIPH_GPIOF);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//输出
	GPIO_InitStruct.Pin = PIN_O_LED;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT; 
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(PORT_O_LED, &GPIO_InitStruct);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//输入
	GPIO_InitStruct.Pin = PIN_I_IR;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT; 
	LL_GPIO_Init(PORT_I_IR, &GPIO_InitStruct);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

}	

/**
	系统时钟配置
	SYSCLK 	= HSI = 24MHz
	AHB 	= SYSCLK/AHBPresc	= 24M
	PCLK 	= AHB/APB1Presc		= 24M
	LPTIM	= LSI(32.768KHz)	= 32.768K

  */
void APP_SystemClockConfig(void)
{
#ifdef SYS_CLOCK_HSE	
	////////////////////////////////////////////////////////////////
	// HSE使能及初始化 
	LL_RCC_HSE_Enable();
	LL_RCC_HSE_SetFreqRegion(LL_RCC_HSE_16_32MHz);
	while(LL_RCC_HSE_IsReady() != 1)
	{
	}

	// 设置AHB分频 
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

	// 配置HSE为系统时钟及初始化 
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE)
	{
	}	
	////////////////////////////////////////////////////////////////
#else
	////////////////////////////////////////////////////////////////
	// 使能HSI 
	LL_RCC_HSI_Enable();
	LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);    
	//LL_RCC_SetHSIDiv
	while(LL_RCC_HSI_IsReady() != 1);

	
	// 设置 AHB 分频
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

	// 配置HSISYS作为系统时钟源 
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSISYS);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSISYS);	
	////////////////////////////////////////////////////////////////
#endif	
	
	
	// 设置flash等待时间
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

	/* 设置 APB1 分频*/
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	
	/* 更新系统时钟全局变量SystemCoreClock(也可以通过调用SystemCoreClockUpdate函数更新) */
	LL_SetSystemCoreClock(24000000);

	LL_Init1msTick(24000000);
	
	//启动SysTick定时器中断
	SysTick_Config(SystemCoreClock/1000);	
}


void APP_ConfigTIM1(void)
{
	/*使能TIM1时钟*/
	LL_APB1_GRP2_EnableClock(RCC_APBENR2_TIM1EN);
	
	
	/*配置TIM1*/
	LL_TIM_InitTypeDef TIM1CountInit = {0};

	TIM1CountInit.ClockDivision       = LL_TIM_CLOCKDIVISION_DIV1;	/* 不分频             */
	TIM1CountInit.CounterMode         = LL_TIM_COUNTERMODE_UP;    	/* 计数模式：向上计数 */
	TIM1CountInit.Prescaler           = 24-1;                   	/* 时钟预分频：   */
	TIM1CountInit.Autoreload          = 25-1;                		/* 自动重装载值：*/
	TIM1CountInit.RepetitionCounter   = 0;                        	/* 重复计数值：0      */

	/*初始化TIM1*/
	LL_TIM_Init(TIM1,&TIM1CountInit);

	/*清除更新标志位*/
	LL_TIM_ClearFlag_UPDATE(TIM1);

	/*使能自动重载预装载*/
	LL_TIM_EnableARRPreload(TIM1);

	/*使能UPDATE中断*/
	LL_TIM_EnableIT_UPDATE(TIM1);

	/*使能TIM1计数器*/
	LL_TIM_EnableCounter(TIM1);

	/*开启UPDATE中断请求*/
	NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
	NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn,0);
}

//1ms  SysTick定时器
void OsTimeTaskCB(void)
{
	if(g_cnt_Timer_T1ms>0)
		g_cnt_Timer_T1ms--;
	
	if(cnt_1sec>0)
		cnt_1sec--;
	
	if(cnt_1sec==0)
	{
		cnt_1sec = 1000;
		
		cnt_sec++;
		b_update = 1;
		if(cnt_sec>59)
		{
			
			cnt_sec = 0;
			cnt_min++;
			if(cnt_min>59)
			{
				cnt_min = 0;
				cnt_hour++;
				if(cnt_hour>99)
					cnt_hour = 0;
			}
		}
		
	}
	
	if(g_cnt_Timer_Led>0)
	{
		g_cnt_Timer_Led--;
		if(g_cnt_Timer_Led==0)
		{
			O_LED_OFF;
		}
	}
			
}


/**
  * @brief  us级延时
  */
void delay_us(uint16_t time)
{
    
    for(;time>0;time--)
    {        
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
        __NOP(); __NOP(); __NOP(); 
    }
}

void delay_ms(uint16_t time)
{
    g_cnt_Timer_T1ms = time;
    
    while(g_cnt_Timer_T1ms>0);	
}	

/**
  * @brief  错误执行函数
  * @param  无
  * @retval 无
  */
void Error_Handler(void)
{
  /* 无限循环 */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  输出产生断言错误的源文件名及行号
  * @param  file：源文件名指针
  * @param  line：发生断言错误的行号
  * @retval 无
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 用户可以根据需要添加自己的打印信息,
     例如: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* 无限循环 */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE****/
