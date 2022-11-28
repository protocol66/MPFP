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
#include "at_cmd.h"


#define DMA_RX_BUFFER_SIZE 100

char rx_string[DMA_RX_BUFFER_SIZE + 1];    // string to hold the received data
char tx_string[1001];    // string to hold the data to be sent
uint32_t rx_len = 0;    // length of the received string



int dma_testing(void) {

    uint16_t data_read;

    while(1)    {

        DELAY_MS(10);
        data_read = DMA_RX_BUFFER_SIZE - fn_usart_get_dma_cntr_rx(&g_st_usart2);

        if (data_read > 0) {
            if (data_read >= DMA_RX_BUFFER_SIZE) {      // if the buffer overflowed nuke it
                rx_len = 0;
            } else if (rx_string[data_read-1] == '\n')  {   // check for end of string delimiter
                rx_len = data_read;
            }

            fn_usart_disable_dma_rx(&g_st_usart2);
            fn_usart_reset_counter_dma_rx(&g_st_usart2, DMA_RX_BUFFER_SIZE);
            fn_usart_enable_dma_rx(&g_st_usart2);

            if (rx_len > 0) {
                rx_string[rx_len] = '\0';                           // terminate string

                if(STRING_CHK_EQ(AT_CMD_RQ_STATUS, rx_string)) {
                    USART2_SEND_STRING(AT_CMD_TS_STATUS_READY);
                } else if (STRING_CHK_EQ(AT_CMD_RC_RESET, rx_string)) {
                    NVIC_SystemReset();
                } else {
                    USART2_SEND_STRING(AT_CMD_TS_CMD_INVALID);
                }

            } else {
                USART2_CLEAR_RX_BUFFER();
                USART2_SEND_STRING("Buffer overflow\n");
            }
            
        }       
    }


    return -1;
}


int main(void)  {
    
    // temporary until clock setup is working
    rcc_ahb_frequency = 4e6;		// clock defaults to 4MHz at startup
    rcc_apb1_frequency = 4e6;
    rcc_apb2_frequency = 4e6;

    __disable_irq();    // no interrupts needed for this homework

    SETUP_USART2(57600);     // setup USART2 for printf

    dma_testing();
    
    return 1;   // should never get here, but if it does return 1 to indicate error, even though all is lost
}