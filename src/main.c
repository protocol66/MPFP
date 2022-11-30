/****************************************
*	Author: 	Jonathan Sanderson		*
*	Date:		FAll 2022				*
*	License:	BSD                  	*
*****************************************
*
*	File: 		main.c
*	Depends: 	j_utils.h
*	Processor: 	STM32L476xx
*	Compiler:	arm-none-eabi gcc 7.2.1
*	
*	NOTES:
*		This is the main file for assignment 2
*		The purpose of this assignment is to demonstrate knowledge of how
*		to configure PWM using timers on the STM32L476xx microcontroller.
*		The assignment was given in 3 parts:  A, B, and C. These parts can be selected
*		by uncommenting the DEFINE statements right below this comment block.
*
***/



#include "lib.h"
#include "string.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"

#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "network.h"
#include "network_data.h"
#include "image.h"


#define DMA_RX_BUFFER_SIZE 100

void SystemClock_Config(void);
int aiInit(void);
int aiRun(const void*, void*);


char rx_string[DMA_RX_BUFFER_SIZE + 1];    // string to hold the received data
char tx_string[1001];    // string to hold the data to be sent
uint32_t rx_len = 0;    // length of the received string

static ai_handle network = AI_HANDLE_NULL;
AI_ALIGNED(4)
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

AI_ALIGNED(4) 
ai_i8 in_data[AI_NETWORK_IN_1_SIZE];
AI_ALIGNED(4) 
ai_i8 out_data[AI_NETWORK_OUT_1_SIZE];\

static ai_buffer* ai_input;
static ai_buffer* ai_output;

int main(void)  {
    // // temporary until clock setup is working
    // rcc_ahb_frequency = 4e6;		// clock defaults to 4MHz at startup
    // rcc_apb1_frequency = 4e6;
    // rcc_apb2_frequency = 4e6;
    SystemClock_Config();  // Initialize the system clock


    // __disable_irq();    // no interrupts needed for this homework

    // SETUP_USART2(57600);     // setup USART2 for printf
    crc_init();             // initialize CRC peripheral

    SETUP_LD2();

    while (1) {
        // copy the image data into the input buffer
        memcpy(in_data, image_data, AI_NETWORK_IN_1_SIZE);

        // run the AI
        aiRun(in_data, out_data);

        // get the prediction (the index of the highest value in the output buffer)
        int prediction = 0;
        for (int i = 0; i < AI_NETWORK_OUT_1_SIZE; i++) {
            if (out_data[i] > out_data[prediction]) {
                prediction = i;
            }
        }

        // turn on LED if the prediction is correct
        if (prediction == (int) label) {
            TURN_ON_LD2();
        } else {
            TURN_OFF_LD2();
        }

    }
    
    return 1;   // should never get here, but if it does return 1 to indicate error, even though all is lost
}

// System clock configuration
void SystemClock_Config() {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    LL_RCC_MSI_Enable();
    while (LL_RCC_MSI_IsReady() != 1) {
        // Wait for MSI ready
    };
    
    // PLL configuration
    // PLLCLK = MSI(4 MHz) / 1 * 40 / 2 = 80 MHz
    uint32_t PLLM = LL_RCC_PLLM_DIV_1;
    uint32_t PLLN = 40;
    uint32_t PLLR = LL_RCC_PLLR_DIV_2;
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, PLLM, PLLN, PLLR);
    LL_RCC_PLL_Enable();
    LL_RCC_PLL_EnableDomain_SYS();
    while (LL_RCC_PLL_IsReady() != 1) {
        // Wait for PLL ready
    };

    // Set AHB prescaler
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
        // Wait for system clock switch to PLL
    };

    // Set APB1 & APB2 prescaler
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    // Set systick to 1ms
    LL_Init1msTick(__LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_MSI_FREQ(LL_RCC_MSIRANGESEL_RUN, LL_RCC_MSI_GetRange()), PLLM, PLLN, PLLR));
    // LL_Init1msTick(80000000);
    // Update CMSIS variable
    LL_SetSystemCoreClock(__LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_MSI_FREQ(LL_RCC_MSIRANGESEL_RUN, LL_RCC_MSI_GetRange()), PLLM, PLLN, PLLR));
    // LL_SetSystemCoreClock(80000000);
}

int aiInit() {
    ai_error err;
    
    const ai_handle acts[] = {activations};
    err = ai_network_create_and_init(&network, acts, NULL);
    if (err.type != AI_ERROR_NONE) {
        return -1;
    }

    ai_input = ai_network_inputs_get(network, NULL);
    ai_output = ai_network_outputs_get(network, NULL);
    return 0;
}

int aiRun(const void *in_data, void *out_data) {
    ai_i32 n_batch;
    ai_error err;

    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    n_batch = ai_network_run(network, &ai_input[0], &ai_output[0]);
    if (n_batch != 1) {
        err = ai_network_get_error(network);
        return -1;
    };

    return 0;
}