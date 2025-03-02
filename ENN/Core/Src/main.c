/****************************************
*	Author: 	Jonathan Sanderson & Parth Pate
*	Date:		FAll 2022				
*	License:	BSD                  	
*****************************************
*
*	File: 		main.c
*	Processor: 	STM32L476xx
*	Compiler:	arm-none-eabi gcc 7.2.1
*	
***/



#include "lib.h"
#include "at_cmd.h"
#include "seven_seg.h"
// #include "string.h"
#include "stdio.h"

#include "ai_datatypes_defines.h"
#include "ai_platform.h"
#include "network.h"
#include "network_data.h"
// #include "image.h"


#define IMAGE_SIZE ((uint16_t)3*32*32)
#define DMA_RX_BUFFER_SIZE (IMAGE_SIZE + 32)   // extra bytes just in case...

void SystemClock_Config(void);
int aiInit(void);
int aiRun(const void*, void*);
void data_ctrl_init(void);
uint8_t* data_ctrl_get_img(void);
void seven_seg_init(Seven_Seg*);
uint8_t process_img(uint8_t*);

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

// AI data structures
static ai_handle network = AI_HANDLE_NULL;
AI_ALIGNED(4)
static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

AI_ALIGNED(4) 
ai_float in_data[AI_NETWORK_IN_1_SIZE];
AI_ALIGNED(4) 
ai_float out_data[AI_NETWORK_OUT_1_SIZE];

static ai_buffer* ai_input;
static ai_buffer* ai_output;

int main(void)  {
    __disable_irq(); 
    fn_system_clock_config_80MHz();  // Initialize the system clock
    crc_init();             // initialize CRC peripheral       
    SETUP_USART2(115200);     // setup USART2 

    Seven_Seg seven_seg;
    aiInit();
    data_ctrl_init();
    seven_seg_init(&seven_seg);

    __enable_irq();     // enable interrupts

    while (1) {
        // obtain image then process it
        uint8_t *img = data_ctrl_get_img();
        uint8_t pred = process_img(img);
        // display prediction
        fn_seven_seg_display(&seven_seg, pred, 1);
    }
    
    return 1;   // should never get here, but if it does return 1 to indicate error, even though all is lost
}


// Initialize neural network
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

// Run neural network
int aiRun(const void *in_data, void *out_data) {
    ai_i32 n_batch;
    ai_error err;

    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    n_batch = ai_network_run(network, &ai_input[0], &ai_output[0]);
    if (n_batch != 1) {
        err = ai_network_get_error(network);
        if (err.type != AI_ERROR_NONE) {
            return -1;
        }
    };

    return 0;
}

// Initialize data control
void data_ctrl_init(void)    {
    // setup DMA for USART2
    fn_usart_disable_dma_rx(&g_st_usart2);
    fn_setup_usart_dma_rx(&g_st_usart2, dma_rx_active_buffer, DMA_RX_BUFFER_SIZE);
    fn_usart_enable_dma_rx(&g_st_usart2);

    // enable intrupts for receiving commands
    enable_usart_interupt();
}

// Get image from data control
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

// Process image
uint8_t process_img(uint8_t* img)  {
    // copy the image data into the input buffer
    for (int i = 0; i < AI_NETWORK_IN_1_SIZE; i++) {
        in_data[i] = (ai_float) img[i];
    }

    // run the AI
    aiRun(in_data, out_data);

    // get the prediction (the index of the highest value in the output buffer)
    uint8_t prediction = 0;
    for (uint8_t i = 0; i < AI_NETWORK_OUT_1_SIZE; i++) {
        if (out_data[i] > out_data[prediction]) {
            prediction = i;
        }
    }
    return prediction;
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
