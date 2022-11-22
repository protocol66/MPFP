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


#define DMA_RX_BUFFER_SIZE 100

char rx_string[DMA_RX_BUFFER_SIZE + 1];    // string to hold the received data
char tx_string[1001];    // string to hold the data to be sent
uint32_t rx_len = 0;    // length of the received string


int main(void)  {

    // temorary until clock setup is working
    rcc_ahb_frequency = 4e6;		// clock defautls to 4MHz at startup
    rcc_apb1_frequency = 4e6;
    rcc_apb2_frequency = 4e6;

    __disable_irq();    // no interupts needed for this homework

    SETUP_USART2(9600);     // setup USART2 for printf
    fn_setup_usart_dma_rx(&g_st_usart2, rx_string, DMA_RX_BUFFER_SIZE);    // setup DMA for USART2 RX

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

        

    }
    
    return 1;   // should never get here, but if it does return 1 to indicate error, even though all is lost
}