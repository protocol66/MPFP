#ifndef TIMER_H
#define TIMER_H

#include <stm32l4xx.h>				// processor defines
#include <stm32l4xx_ll_bus.h>		// needed for enabling clock to peripherals
#include <stm32l4xx_ll_tim.h>		// needed to configure the timers

#include "common.h"

/**
*	@brief This function looks up the timer pointer from given integer
* 
*	@param u32_timer_sel integer corisponding to the timer to be returned
*
*	@retval TIM_TypeDef* pointer to the timer, NULL if not found
*
* 	@note Precondition:	None
*/
__STATIC_INLINE
TIM_TypeDef* __INT_TO_TIM(uint32_t u32_timer_sel)	{
	switch (u32_timer_sel)	{
		case 1:
			return TIM1;
		case 2:
			return TIM2;
		case 3:
			return TIM3;
		case 4:
			return TIM4;
		case 5:
			return TIM5;
		case 6:
			return TIM6;
		case 7:
			return TIM7;
		case 8:
			return TIM8;
		case 15:
			return TIM15;
		case 16:
			return TIM16;
		case 17:
			return TIM17;
		default:
			return NULL;
	}
}

/**
*	@brief This function looks up the timer channel pointer from given integer
* 
*	@param u32_channel integer corisponding to the channel to be returned
*
*	@retval uint32_t pointer to the channel, -1 (u32 max) if not found
*
* 	@note Precondition:	None
*/
__STATIC_INLINE
uint32_t __INT_TO_TIM_CHAN(uint32_t u32_channel)	{
	switch (u32_channel)	{
		case 1:
			return LL_TIM_CHANNEL_CH1;
			break;
		case 2:
			return LL_TIM_CHANNEL_CH2;
			break;
		case 3:
			return LL_TIM_CHANNEL_CH3;
			break;
		case 4:
			return LL_TIM_CHANNEL_CH4;
			break;
		default:
			return (uint32_t)(-1);	// error
			break;
	}
}

__STATIC_INLINE
int __IS_TIM_32BIT(TIM_TypeDef* pst_timer)	{
	if ((pst_timer == TIM2) || (pst_timer == TIM5))	{
		return 1;
	} else {
		return 0;
	}
}


/**
*	@brief This functions period of the timer in Hz
* 
*	@param u32_timer timer to be changed, (2-5) 
*	@param u32_hz the frequency to be set, min = 1, max = rcc_apb1_frequency
*
*	@retval			None
*
* 	@note Precondition:	fn_setup_timer()
* 	@note Impact: changes the timer auto reload and prescaler
*/
void fn_set_timer_hz(uint32_t u32_timer, uint32_t u32_hz)  {
	TIM_TypeDef *pst_timer = __INT_TO_TIM(u32_timer);

	if(__IS_TIM_32BIT(pst_timer))	{
		LL_TIM_SetPrescaler(pst_timer, 0);
		LL_TIM_SetAutoReload(pst_timer, (rcc_apb1_frequency/u32_hz) - 1);
	} else {
		if (u32_hz < 1e3)	{
			LL_TIM_SetPrescaler(pst_timer, (rcc_apb1_frequency/5e4) - 1);		// scale clock to 50kHz
			LL_TIM_SetAutoReload(pst_timer, (50e3/u32_hz) - 1);
		} else if (u32_hz < 1e6)	{
			LL_TIM_SetPrescaler(pst_timer, (rcc_apb1_frequency/5e7) - 1);		// scale clock to 50MHz
			LL_TIM_SetAutoReload(pst_timer, (50e6/u32_hz) - 1);
		} else {
			LL_TIM_SetPrescaler(pst_timer, 0);
			LL_TIM_SetAutoReload(pst_timer, (rcc_apb1_frequency/u32_hz) - 1);
		}
	}
}


/**
*	@brief This functions period of the timer in us
* 
*	@param u32_timer timer to be changed, (2-5) 
*	@param u32_us the period to be set, min = 1 (clock must be at least 1MHz), max = 1sec
*
*	@retval			None
*
* 	@note Precondition:	fn_setup_timer()
* 	@note Impact: changes the timer auto reload and prescaler
*/
void fn_set_timer_us(uint32_t u32_timer, uint32_t u32_us)  {
	fn_set_timer_hz(u32_timer, 1e6/u32_us);
}


/**
*	@brief This functions period of the timer in ms
* 
*	@param u32_timer timer to be changed, (2-5) 
*	@param u32_us the period to be set, min = 1, max = 1sec
*
*	@retval			None
*
* 	@note Precondition:	fn_setup_timer()
* 	@note Impact: changes the timer auto reload and prescaler
*/
void fn_set_timer_ms(uint32_t u32_timer, uint32_t u32_us)  {
	fn_set_timer_hz(u32_timer, 1e3/u32_us);
}

/**
*	@brief This functions period of the timer in seconds
* 
*	@param u32_timer timer to be changed, (2-5) 
*	@param u32_sec the period to be set, min = 1sec, max = 50sec (16bit timer), 3.5e6 sec (32bit timer)
*
*	@retval			None
*
* 	@note Precondition:	fn_setup_timer()
* 	@note Impact: changes the timer auto reload and prescaler
*/
void fn_set_timer_sec(uint32_t u32_timer, uint32_t u32_sec)  {
	TIM_TypeDef *pst_timer = __INT_TO_TIM(u32_timer);

	LL_TIM_SetPrescaler(pst_timer, 65535);		// max prescaler, divide by 65536
	LL_TIM_SetAutoReload(pst_timer, ((u32_sec * (rcc_apb1_frequency >> 16)) - 1));		// use fast division
}


/**
*	@brief This functions changes the duty cycle of a timer
* 
*	@param u32_timer timer to be changed, (2-5) 
*	@param u32_duty duty cycle in thousandths aka 12.34% = 1234
*	@param u32_channel channel to be changed (1-4)
*
*	@retval			None
*
* 	@note Precondition:	fn_setup_timer()
* 	@note Impact: changes timer channel config
*/
void fn_set_timer_duty(uint32_t u32_timer, uint32_t u32_duty, uint32_t u32_channel)  {
	TIM_TypeDef *pst_timer = __INT_TO_TIM(u32_timer);
	uint32_t u32_period = LL_TIM_GetAutoReload(pst_timer);	

	u32_duty = (u32_period * u32_duty) / 10000;
	
	switch (u32_channel)	{
		case 1:
			LL_TIM_OC_SetCompareCH1(pst_timer, u32_duty);
			break;
		case 2:
			LL_TIM_OC_SetCompareCH2(pst_timer, u32_duty);
			break;
		case 3:
			LL_TIM_OC_SetCompareCH3(pst_timer, u32_duty);
			break;
		case 4:
			LL_TIM_OC_SetCompareCH4(pst_timer, u32_duty);
			break;
		default:
			break;
	}
}

/**
*	@brief This functions sets up a given channel of a timer
* 
*	@param u32_timer timer number (2-5), 
*	@param u32_channel channel to setup (1-4)
*
*	@retval		0 = success, -1 = error (channel not found)
*
* 	@note Precondition:	None
* 	@note Impact: changes timer channel config
* 	@note This function changes operation of a given channel of a timer
*/
int fn_set_timer_channel_output(uint32_t u32_timer, uint32_t u32_channel)	{
	uint32_t u32_channel_def = __INT_TO_TIM_CHAN(u32_channel);
	TIM_TypeDef *pst_timer_def = __INT_TO_TIM(u32_timer);

	LL_TIM_OC_EnablePreload(pst_timer_def, u32_channel_def);

	LL_TIM_OC_SetMode(pst_timer_def, u32_channel_def, LL_TIM_OCMODE_PWM1);		// set to PWM output, active counter < CCR1	
	LL_TIM_CC_EnableChannel(pst_timer_def, u32_channel_def);

	return 0;	// success
}

/*
*	Precondition:	fn_setup_clock()
*	Input:			timer to setup (2-5)
*	Output:			None
*   Errors:         None
*	Impact:			changes timer config, resets counter
*
* 	Description:
* 		This function setups a timmer with a defualt frequecy of 1KHz and a duty cycle of 50%
*/
void fn_setup_timer_output(uint32_t u32_timer)  {
	TIM_TypeDef *pst_timer = __INT_TO_TIM(u32_timer);

	switch (u32_timer)	{
		case 2:
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
			break;
		case 3:
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
			break;
		case 4:
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
			break;
		case 5:
			LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
			break;
		default:
			break;
	}

	LL_TIM_EnableARRPreload(pst_timer);		// buffer ARR register
	LL_TIM_SetCounterMode(pst_timer, LL_TIM_COUNTERMODE_UP);		// count up	

	fn_set_timer_channel_output(u32_timer, 1);
	fn_set_timer_hz(u32_timer, 1000);	// set to 1KHz
	fn_set_timer_duty(u32_timer, 5000, 1);	// set duty cycle to 50.00%

	LL_TIM_EnableCounter(pst_timer);		// enable timer
	LL_TIM_GenerateEvent_UPDATE(pst_timer);		// kick start the timer

}

#endif