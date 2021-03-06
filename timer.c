/* timer.c */
#include <timer.h>


/**
	* \brief This function sets up timer TIM4 peripheral 
	*
	*/
void TimerInit(void)
{
	RCC_ClocksTypeDef RCC_Clocks;
	uint32_t APB1_CLK;
	uint16_t TimerPeriod;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef timerInitStruct;
	TIM_OCInitTypeDef outputConfig;

	
	//Clock for peripheral device 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	//Get AHB1 frequency 
	RCC_GetClocksFreq(&RCC_Clocks);
	APB1_CLK = RCC_Clocks.PCLK1_Frequency;
	
	//one timer period is 1/TIMER_FREQUENCY seconds 
	TimerPeriod = (uint16_t)((2*APB1_CLK)/((PRESCALER+1)*TIMER_FREQUENCY));
		
	timerInitStruct.TIM_Prescaler 	= PRESCALER;
	timerInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStruct.TIM_Period 			= TimerPeriod;
	timerInitStruct.TIM_ClockDivision = 0;
	timerInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &timerInitStruct);
	
	//Set output compare mode 
	TIM_OCStructInit(&outputConfig);	//Default init 
		outputConfig.TIM_OCMode 			= TIM_OCMode_Active;
		outputConfig.TIM_Pulse				= PERIOD;	//Initial OCR is whole period		
	TIM_OC1Init(TIM4,&outputConfig);
	
	//Configuring timer interrupt sources 
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC2);
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC3);
	TIM_ClearITPendingBit(TIM4,TIM_IT_CC4);
	TIM_SetCounter(TIM4,0);
	TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_Update, DISABLE);
	TIM_ITConfig(TIM4,TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM4,TIM_IT_CC4, ENABLE);
	
	//Enable TIM4 interupt in NVIC
	
		NVIC_InitStruct.NVIC_IRQChannel 									= TIM4_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 5;		//Because it uses FreeRtos functions in ISR
		NVIC_InitStruct.NVIC_IRQChannelSubPriority				= 0;
		NVIC_InitStruct.NVIC_IRQChannelCmd								= ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//Start timer 
	TIM_Cmd(TIM4, ENABLE);
}
