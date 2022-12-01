#ifndef CLOCK_H
#define CLOCK_H


#include <stm32l4xx.h>				// processor defines
#include <stm32l4xx_ll_bus.h>		// needed for enabling clock to peripherals
#include <stm32l4xx_ll_rcc.h>		// needed to setup clock

#include "common.h"

/*
*	Precondition:	None
*	Input:			u32_frequency in Hz
*	Output:			None
*	Impact:			Permently changes sysclock frequency
*
* 	Description:
* 		input must be interger in Mhz where 8MHz <= freq <= 80Mhz
* 		if freq < 8Mhz the system uses MSI clock which has specific freqencies, see switch statment
*/
// void fn_setup_clock(uint32_t u32_freq)	{
// 	// USE THE HSI16
// 	rcc_osc_on(RCC_HSI16);
// 	LL_RCC_OSC
	
// 	flash_prefetch_enable();
// 	flash_set_ws(4);
// 	flash_dcache_enable();
// 	flash_icache_enable();

// 	if(u32_freq >= (uint32_t)8e6)	{
// 		uint32_t u32_PLLM = 4;
// 		uint32_t u32_PLLN = u32_freq / (uint32_t)1e6;
// 		rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, u32_PLLM, u32_PLLN,RCC_PLLCFGR_PLLP_DIV7, RCC_PLLCFGR_PLLQ_DIV4, RCC_PLLCFGR_PLLR_DIV4);

// 		rcc_osc_on(RCC_PLL);
// 		/* either rcc_wait_for_osc_ready() or do other things */
// 		rcc_wait_for_osc_ready(RCC_PLL);
// 		/* Enable clocks for the ports we need */
// 		rcc_set_sysclk_source(RCC_CFGR_SW_PLL); /* careful with the param here! */
// 		rcc_wait_for_sysclk_status(RCC_PLL);

// 	} else {
// 		rcc_osc_off(RCC_MSI);

// 		switch(u32_freq)	{
// 			case 100000:
// 				rcc_set_msi_range(RCC_CR_MSIRANGE_100KHZ);	break;
// 			case 200000:
// 				rcc_set_msi_range(RCC_CR_MSIRANGE_200KHZ);	break;
// 			case 400000:
// 				rcc_set_msi_range(RCC_CR_MSIRANGE_400KHZ);	break;
// 			case 800000:
// 				rcc_set_msi_range(RCC_CR_MSIRANGE_800KHZ);	break;
// 			case 1000000:
// 				rcc_set_msi_range(RCC_CR_MSIRANGE_1MHZ);	break;
// 			case 2000000:
// 				rcc_set_msi_range(RCC_CR_MSIRANGE_2MHZ);	break;
// 			case 4000000:
// 				rcc_set_msi_range(RCC_CR_MSIRANGE_4MHZ);	break;
// 			default:
// 				return;
// 		}

// 		rcc_osc_on(RCC_MSI);
// 		rcc_wait_for_osc_ready(RCC_MSI);
// 		/* Enable clocks for the ports we need */
	
// 		rcc_set_sysclk_source(RCC_CFGR_SW_MSI); /* careful with the param here! */
// 		rcc_wait_for_sysclk_status(RCC_MSI);
// 	}


// 	/* MAGIC NUMBERS ARE EVIL */
	// rcc_ahb_frequency = u32_freq;
	// rcc_apb1_frequency = u32_freq;
	// rcc_apb2_frequency = u32_freq;
// }




/*
*	Precondition:	fn_setup_clock
*	Input:			None
*	Output:			None
*	Impact:			changes GPIO configuration
*
* 	Description:
* 		this functions enable MCO output on a GPIO pin
*/
// extern inline
// void ENABLE_MCO(void)	{
// 	RCC_CFGR |= 0x41000000;		// TODO: NEED to get rid of magic numbers
// 	// rcc_set_mco(RCC_CFGR_MCO_SYSCLK);
// 	rcc_periph_clock_enable(RCC_GPIOA);
// 	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);
// 	gpio_set_af(GPIOA, GPIO_AF0, GPIO8);
// }


#endif