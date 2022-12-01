/****************************************
*	Author: 	Parth Patel      		*
*               Jonathan Sanderson      *
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

#include <stdbool.h>

#include "lib.h"
#include "at_cmd.h"
#include "seven_seg.h"

#define IMAGE_SIZE ((uint16_t)3*32*32)
#define DMA_RX_BUFFER_SIZE (IMAGE_SIZE + 32)   // extra bytes just in case...


__STATIC_INLINE
void enable_usart_interupt(void)  {
    LL_USART_EnableIT_RXNE(g_st_usart2.pst_usart_sel);
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 0);
}

__STATIC_INLINE
void disable_usart_interupt(void)  {
    LL_USART_DisableIT_RXNE(g_st_usart2.pst_usart_sel);
    NVIC_DisableIRQ(USART2_IRQn);
}


uint8_t dma_rx_buffer_1[DMA_RX_BUFFER_SIZE + 1];   // +1 for null terminator
uint8_t dma_rx_buffer_2[DMA_RX_BUFFER_SIZE + 1];
uint8_t *dma_rx_active_buffer = dma_rx_buffer_1;
bool dma_rx_buffer_ready = true;
bool system_error = false;


void sys_init(void)    {
    __disable_irq();    // no interrupts needed for this homework
    fn_system_clock_config_80MHz();
    SETUP_USART2(115200);     // setup USART2 for printf
}

void ai_init(void)  {
    // TODO:  Initialize the AI :)
}

void data_ctrl_init(void)    {
    // setup DMA for USART2
    fn_usart_disable_dma_rx(&g_st_usart2);
    fn_setup_usart_dma_rx(&g_st_usart2, dma_rx_active_buffer, DMA_RX_BUFFER_SIZE);
    fn_usart_enable_dma_rx(&g_st_usart2);

    // enable intrupts for receiving commands
    enable_usart_interupt();
}


uint8_t* data_ctrl_get_img(void) {

    while(dma_rx_buffer_ready);        // wait for image to start being loaded into the buffer

    uint32_t data_read = 0;

    // loop till we get a full image
    while(data_read < IMAGE_SIZE)   {
        data_read = DMA_RX_BUFFER_SIZE - fn_usart_get_dma_cntr_rx(&g_st_usart2);
    }

    // disable dma
    fn_usart_disable_dma_rx(&g_st_usart2);
    fn_usart_reset_counter_dma_rx(&g_st_usart2, DMA_RX_BUFFER_SIZE);

    // swap buffers
    uint8_t * image_ready;
    if (dma_rx_active_buffer == dma_rx_buffer_1) {
        dma_rx_active_buffer = dma_rx_buffer_2;
        image_ready = dma_rx_buffer_1;
    } else {
        dma_rx_active_buffer = dma_rx_buffer_1;
        image_ready = dma_rx_buffer_2;
    }

    // enable dma
    dma_rx_buffer_ready = true;
    fn_setup_usart_dma_rx(&g_st_usart2, dma_rx_active_buffer, DMA_RX_BUFFER_SIZE);
    fn_usart_enable_dma_rx(&g_st_usart2);
    enable_usart_interupt();

    return image_ready;
}

void seven_seg_init(Seven_Seg *seven_seg) {
    seven_seg->a = GPIO_OUTPUT_DEFAULT(GPIOB, LL_GPIO_PIN_1);
    seven_seg->b = GPIO_OUTPUT_DEFAULT(GPIOB, LL_GPIO_PIN_2);
    seven_seg->c = GPIO_OUTPUT_DEFAULT(GPIOB, LL_GPIO_PIN_3);
    seven_seg->d = GPIO_OUTPUT_DEFAULT(GPIOB, LL_GPIO_PIN_4);
    seven_seg->e = GPIO_OUTPUT_DEFAULT(GPIOB, LL_GPIO_PIN_5);
    seven_seg->f = GPIO_OUTPUT_DEFAULT(GPIOB, LL_GPIO_PIN_6);
    seven_seg->g = GPIO_OUTPUT_DEFAULT(GPIOB, LL_GPIO_PIN_7);
    seven_seg->dot = GPIO_OUTPUT_DEFAULT(GPIOB, LL_GPIO_PIN_8);
    seven_seg->active_low = 0;

    fn_seven_seg_init(seven_seg);
    fn_seven_seg_clear(seven_seg);
}

uint8_t process_img(uint8_t* img)  {
    static uint8_t img_count = 0xff;

    img_count = (img_count + 1) % 16;

    // DELAY_MS(500);             // fake processing time
    return img_count;
}


int main(void)  {
    Seven_Seg seven_seg;

    sys_init();
    ai_init();
    data_ctrl_init();
    seven_seg_init(&seven_seg);

    __enable_irq();     // enable interrupts

    while(1)    {
        uint8_t *img = data_ctrl_get_img();
        uint8_t pred = process_img(img);
        fn_seven_seg_display(&seven_seg, pred, 1);

    }
    
    return 1;   // should never get here, but if it does return 1 to indicate error, even though all is lost
}





/* uart handler to catch commands*/
void USART2_IRQHandler(void)
{
    uint16_t data_read = DMA_RX_BUFFER_SIZE - fn_usart_get_dma_cntr_rx(&g_st_usart2);

    if (data_read == 0)     // this should never happen, but just in case
        return;

    if (data_read >= DMA_RX_BUFFER_SIZE)    {   // dma overflow
        system_error = true;
        USART2_SEND_STRING(AT_CMD_TS_STATUS_ERROR);     // we dont know what they wanted so send error
    }

    if (dma_rx_active_buffer[data_read-1] != '\n')      // we are looking for the newline character as a deliniator
        return;

    dma_rx_active_buffer[data_read] = '\0';     // convert to a string

    if(STRING_CHK_EQ(AT_CMD_RQ_STATUS, (char *)dma_rx_active_buffer)) {
        if(dma_rx_buffer_ready) {
            USART2_SEND_STRING(AT_CMD_TS_STATUS_READY);
        } else {
            USART2_SEND_STRING(AT_CMD_TS_STATUS_BUSY);
        }
    } else if (STRING_CHK_EQ(AT_CMD_RD_IMAGE, (char *)dma_rx_active_buffer)) {      // image is being sent, disable interupt because this will take a while
        if(dma_rx_buffer_ready) {
            dma_rx_buffer_ready = false;
            disable_usart_interupt();
        } else {
            USART2_SEND_STRING(AT_CMD_TS_STATUS_BUSY);
        }
    }
    else if (STRING_CHK_EQ(AT_CMD_RC_RESET, (char *)dma_rx_active_buffer)) {        // the oh sh!t button
        NVIC_SystemReset();
    } else {
        USART2_SEND_STRING(AT_CMD_TS_CMD_INVALID);          // Invalid command - it doesn't speak stupid
    }

    // reset dma counter
    fn_usart_disable_dma_rx(&g_st_usart2);
    fn_usart_reset_counter_dma_rx(&g_st_usart2, DMA_RX_BUFFER_SIZE);
    fn_usart_enable_dma_rx(&g_st_usart2);
            
}