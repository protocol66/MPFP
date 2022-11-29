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

#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "network.h"
#include "network_data.h"


#define DMA_RX_BUFFER_SIZE 100

void SystemClock_Config(void);

char rx_string[DMA_RX_BUFFER_SIZE + 1];    // string to hold the received data
char tx_string[1001];    // string to hold the data to be sent
uint32_t rx_len = 0;    // length of the received string


int main(void)  {
    ai_error ai_err;
    ai_i32 nbatch;  
    float pred;
    // AI setup
    // Chunk memory to hold intermediate layer activations
    AI_ALIGNED(4) ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];
    // Input Output buffers
    AI_ALIGNED(4) ai_i8 in_data[AI_NETWORK_IN_1_SIZE_BYTES];
    AI_ALIGNED(4) ai_i8 out_data[AI_NETWORK_OUT_1_SIZE_BYTES];

    // Create a network handle
    ai_handle network = AI_HANDLE_NULL;

    // Initialize wrapper structs that hold pointers to data and info about the
    // data (tensor height, width, channels)
    ai_buffer ai_input[AI_NETWORK_IN_NUM] = AI_NETWORK_IN;
    ai_buffer ai_output[AI_NETWORK_OUT_NUM] = AI_NETWORK_OUT;

    // Set working memory and get weights/biases from model
    ai_network_params ai_params = {
        AI_NETWORK_DATA_WEIGHTS(ai_network_data_weights_get()),
        AI_NETWORK_DATA_ACTIVATIONS(activations)
    };

    // Set pointers wrapper structs to our data buffers
    // ai_input[0].n_batches = 1;
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    // ai_output[0].n_batches = 1;
    ai_output[0].data = AI_HANDLE_PTR(out_data);


    // // temporary until clock setup is working
    // rcc_ahb_frequency = 4e6;		// clock defaults to 4MHz at startup
    // rcc_apb1_frequency = 4e6;
    // rcc_apb2_frequency = 4e6;
    SystemClock_Config();  // Initialize the system clock


    __disable_irq();    // no interrupts needed for this homework

    SETUP_USART2(57600);     // setup USART2 for printf
    crc_init();             // initialize CRC peripheral

    // Create instance of neural network
    ai_err = ai_netwprl_model_create(&network, AI_NETWORK_DATA_CONFIG);
    if (ai_err.type != AI_ERROR_NONE) {
        while(1);
    }

    // Initialize network
    if (!ai_network_init(network, &ai_params)) {
        // ai_network_destroy(network);
        while(1);
    }

    while (1)
    {
        // DELAY_MS(10);
        // rx_len = USART2_RECEIVE_STRING(rx_string, 100);    // receive a string from the user
        // MEMCOPY(tx_string, rx_string, rx_len);          // copy the received string to the tx string

        // if (rx_len > 0) {
        //     tx_string[rx_len-1] = '\n';                         // add net
        //     tx_string[rx_len] = '\0';                           // add carriage return
        //     USART2_SEND_STRING(tx_string);
        // } else {
        //     USART2_CLEAR_RX_BUFFER();
        //     USART2_SEND_STRING("Buffer overflow\n");
        // }


        // dma test - delay 1s before echoing message
        uint16_t data_read;
        DELAY_MS(1000);
        data_read = DMA_RX_BUFFER_SIZE - fn_usart_disable_dma_rx(&g_st_usart2);

        if (data_read > 0) {
            if (data_read >= DMA_RX_BUFFER_SIZE) {      // if the buffer overflowed nuke it
                fn_usart_reset_counter_dma_rx(&g_st_usart2, DMA_RX_BUFFER_SIZE);
            } else if (rx_string[data_read-1] == '\n')  {   // check for end of string delimiter
                rx_string[data_read] = '\0';        // get ready for transmission
                USART2_SEND_STRING(rx_string);
                fn_usart_reset_counter_dma_rx(&g_st_usart2, DMA_RX_BUFFER_SIZE);
            }
            
        } 

        fn_usart_enable_dma_rx(&g_st_usart2);

        

        if (rx_len > 0) {
            tx_string[rx_len-1] = '\n';                         // add net
            tx_string[rx_len] = '\0';                           // add carriage return
            USART2_SEND_STRING(tx_string);
        } else {
            USART2_CLEAR_RX_BUFFER();
            USART2_SEND_STRING("Buffer overflow\n");
        }

        // I have no idea if this is the right way to do this
        // Read in data from uart
        // for (int i = 0; i < AI_NETWORK_IN_1_SIZE; i++) {
        //     // ((ai_float*)in_data)[i] = (ai_float) rx_string[i];
        // }
    
        // // Run the network
        // nbatch = ai_network_run(network, &ai_input[0], &ai_output[0]);
        // if (nbat)ch != 1) {
        //     // ai_network_destroy(network);
        //     while(1);
        // }
        
        // Get output of network
        // pred = ((float*)out_data)[0];

        // Enable corresponding LED
        
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