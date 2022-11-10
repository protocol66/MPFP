#ifndef GPIO_H
#define GPIO_H

#include <stm32l4xx.h>				// processor defines
#include <stm32l4xx_ll_rcc.h>		// needed to setup clock
#include <stm32l4xx_ll_gpio.h>		// needed to configure gpio

#include "common.h"


/*
* LED and BUTTON defines
*/
#define LED_PIN LL_GPIO_PIN_5
#define LED_PORT GPIOA
#define BUTTON_PIN LL_GPIO_PIN_13
#define BUTTON_PORT GPIOC


/*
	MACROS
*/
#define GPIO_INPUT_DEFAULT(pst_port_, u32_pin_)		((GPIO){.pst_port=pst_port_, .u32_pin=u32_pin_, .u32_mode=LL_GPIO_MODE_INPUT, .u32_up_down=LL_GPIO_PULL_NO, .u32_type=LL_GPIO_OUTPUT_PUSHPULL, .u32_speed=LL_GPIO_SPEED_FREQ_MEDIUM, .u32_af=LL_GPIO_AF_0})
#define GPIO_OUTPUT_DEFAULT(pst_port_, u32_pin_)	((GPIO){.pst_port=pst_port_, .u32_pin=u32_pin_, .u32_mode=LL_GPIO_MODE_OUTPUT, .u32_up_down=LL_GPIO_PULL_NO, .u32_type=LL_GPIO_OUTPUT_PUSHPULL, .u32_speed=LL_GPIO_SPEED_FREQ_MEDIUM, .u32_af=LL_GPIO_AF_0})



/*
*	Simple struct for containing gpio settings
*/
typedef struct {
	GPIO_TypeDef *pst_port;
	uint32_t u32_pin;
	uint32_t u32_mode;
	uint32_t u32_up_down;
	uint32_t u32_type;
	uint32_t u32_speed;
	uint32_t u32_af;
} GPIO;


/*
* Create button and LED structs
*/
GPIO g_st_led;
GPIO g_st_button;


/*
*	Precondition:	fn_setup_clock()
*	Input:			GPIO data type
*	Output:			None
*   Errors:         None
*	Impact:			changes gpio config
*
* 	Description:
* 		This function setups a gpio using the struct above
*/
void fn_setup_gpio(const GPIO *gpio)	{
	/*enables clock for gpio port*/
	switch ((uint32_t)gpio->pst_port)	{
	case (uint32_t)GPIOA:
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
		break;
	case (uint32_t)GPIOB:
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
		break;
	case (uint32_t)GPIOC:
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
		break;
	case (uint32_t)GPIOD:
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
		break;
	case (uint32_t)GPIOE:
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
		break;
	case (uint32_t)GPIOF:
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);
		break;
	case (uint32_t)GPIOH:
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);
		break;
	default:
		return;
		break;
	}

	LL_GPIO_SetPinOutputType(gpio->pst_port, gpio->u32_pin, gpio->u32_type);
	LL_GPIO_SetPinMode(gpio->pst_port, gpio->u32_pin, gpio->u32_mode);
	LL_GPIO_SetPinSpeed(gpio->pst_port, gpio->u32_pin, gpio->u32_speed);
	LL_GPIO_SetPinPull(gpio->pst_port, gpio->u32_pin, gpio->u32_up_down);
	
	if (gpio->u32_pin <= LL_GPIO_PIN_7)	{
		LL_GPIO_SetAFPin_0_7(gpio->pst_port, gpio->u32_pin, gpio->u32_af);
	} else {
		LL_GPIO_SetAFPin_8_15(gpio->pst_port, gpio->u32_pin, gpio->u32_af);
	}
}



/*
*	Precondition:	fn_setup_gpio()
*	Input:			gpio struct
*	Output:			None
*	Impact:			Will set GPIO pin
*
* 	Description:
* 		this function uses the gpio struct to set the pin (active high)
*/
__STATIC_INLINE
void SET_GPIO_PIN(const GPIO gpio)	{
	LL_GPIO_SetOutputPin(gpio.pst_port, gpio.u32_pin);
}

/*
*	Precondition:	fn_setup_gpio()
*	Input:			gpio struct
*	Output:			None
*	Impact:			Will clear GPIO pin
*
* 	Description:
* 		this function uses the gpio struct to clear the pin (active high)
*/
__STATIC_INLINE
void CLEAR_GPIO_PIN(const GPIO gpio)	{
	LL_GPIO_ResetOutputPin(gpio.pst_port, gpio.u32_pin);
}

/*
*	Precondition:	fn_setup_gpio()
*	Input:			gpio struct
*	Output:			None
*	Impact:			Will toggle GPIO pin
*
* 	Description:
* 		this function uses the gpio struct to toggle the pin
*/
__STATIC_INLINE
int READ_GPIO_PIN(const GPIO gpio)	{
	return LL_GPIO_ReadInputPort(gpio.pst_port) & gpio.u32_pin;
}

/*
*	Precondition:	fn_setup_clock is setup
*	Input:			None
*	Output:			None
*	Impact:			changes GPIO configuration
*
* 	Description:
* 		this functions initilizes LED 2 gpio for output
*/
__STATIC_INLINE
void SETUP_LD2(void)
{
	g_st_led.pst_port = LED_PORT;
	g_st_led.u32_pin = LED_PIN;
	g_st_led.u32_mode = LL_GPIO_MODE_OUTPUT;
	g_st_led.u32_up_down = LL_GPIO_PULL_NO;
	g_st_led.u32_speed = LL_GPIO_SPEED_FREQ_MEDIUM;
	g_st_led.u32_type = LL_GPIO_OUTPUT_PUSHPULL;

	fn_setup_gpio(&g_st_led);
}

/*
*	Precondition:	SETUP_LD2
*	Input:			None
*	Output:			None
*	Impact:			changes GPIO configuration
*
* 	Description:
* 		this functions sets LED 2 high
*/
__STATIC_INLINE
void TURN_ON_LD2(void)	{
	SET_GPIO_PIN(g_st_led);
}

/*
*	Precondition:	SETUP_LD2
*	Input:			None
*	Output:			None
*	Impact:			changes GPIO configuration
*
* 	Description:
* 		this functions sets LED 2 low
*/
__STATIC_INLINE
void TURN_OFF_LD2(void)	{
	CLEAR_GPIO_PIN(g_st_led);
}

/*
*	Precondition:	SETUP_LD2
*	Input:			None
*	Output:			None
*	Impact:			changes GPIO configuration
*
* 	Description:
* 		this functions toggles LED 2
*/
__STATIC_INLINE
void TOGGLE_LD2(void)	{
	if (READ_GPIO_PIN(g_st_led))	{
		CLEAR_GPIO_PIN(g_st_led);
	} else {
		SET_GPIO_PIN(g_st_led);
	}
}

/*
*	Precondition:	SETUP_LD2
*	Input:			None
*	Output:			int
*	Impact:			None
*
* 	Description:
* 		reads LED 2 state
*/
__STATIC_INLINE
int IS_LD2_ON(void)	{
	return READ_GPIO_PIN(g_st_led);
}

/*
*	Precondition:	SETUP_LD2
*	Input:			None
*	Output:			int
*	Impact:			None
*
* 	Description:
* 		reads LED 2 state
*/
__STATIC_INLINE
int IS_LD2_OFF(void)	{
	return !READ_GPIO_PIN(g_st_led);
}

/*
*	Precondition:	fn_setup_sysclock
*	Input:			None
*	Output:			None
*	Impact:			changes GPIO configuration
*
* 	Description:
* 		setups button 1 for input
*/
__STATIC_INLINE
void SETUP_BUTTON(void)	{
	g_st_button.pst_port = BUTTON_PORT;
	g_st_button.u32_pin = BUTTON_PIN;
	g_st_button.u32_mode = LL_GPIO_MODE_INPUT;
	g_st_button.u32_up_down = LL_GPIO_PULL_NO;
	g_st_button.u32_speed = LL_GPIO_SPEED_FREQ_LOW;
	g_st_button.u32_type = LL_GPIO_OUTPUT_PUSHPULL;


	fn_setup_gpio(&g_st_button);
}

/*
*	Precondition:	SETUP_BUTTON
*	Input:			None
*	Output:			int
*	Impact:			None
*
* 	Description:
* 		reads button 1 state
*/
__STATIC_INLINE
int IS_B1_PRESSED(void)	{
	return !READ_GPIO_PIN(g_st_button);
}

/*
*	Precondition:	SETUP_BUTTON
*	Input:			None
*	Output:			int
*	Impact:			None
*
* 	Description:
* 		reads button 1 state
*/
__STATIC_INLINE
int IS_B1_RELEASED(void)	{
	return READ_GPIO_PIN(g_st_button);
}

#endif