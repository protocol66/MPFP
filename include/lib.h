/****************************************
*	Author: 	Jonathan Sanderson		*
*	Date:		FALL 2022				*
*	License:	BSD                  	*
*****************************************
*
*	File: 		j_utils.h
*	Depends: 	stm32lf4xx.h, stm32l4xx_ll library
*	Processor: 	STM32L476xx
*	Compiler:	arm-none-eabi gcc 7.2.1
*	
*	NOTES:
*		This file contains macros and helper functions
*		to aid in programming for the STM32L4. It is 
*		prone to change very often.
*
***/

#ifndef J_UTILS_H
#define J_UTILS_H


// Defines and global variables
#include "common.h"

// peripheral includes
#include "clock.h"
#include "gpio.h"
#include "timer.h"
#include "uart.h"
#include "crc.h"

// device drivers
#include "keypad.h"


#endif