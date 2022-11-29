#ifndef UART_H
#define UART_H

#include <stm32l4xx.h>				// processor defines
#include <stm32l4xx_ll_bus.h>		// needed for enabling clock to peripherals
#include <stm32l4xx_ll_usart.h>		// needed to configure the uart
#include <stm32l4xx_ll_dma.h>		// needed to configure the DMA

#include "common.h"
#include "gpio.h"

#define BUTTON_PIN LL_GPIO_PIN_13
#define BUTTON_PORT GPIOC
#define USART2_TX_PIN LL_GPIO_PIN_2
#define USART2_TX_PORT GPIOA
#define USART2_RX_PIN LL_GPIO_PIN_3
#define USART2_RX_PORT GPIOA
#define USART2_TX_AF LL_GPIO_AF_7
#define USART2_RX_AF LL_GPIO_AF_7


/*
*	This stuct contains all the main settings for usart setup
*/
typedef struct {
	GPIO *pst_gpio_tx;
	GPIO *pst_gpio_rx;
	USART_TypeDef * pst_usart_sel;
	uint32_t u32_baudrate;
	uint32_t u32_data_bits;
	uint32_t u32_stop_bits;
	uint32_t u32_mode;
	uint32_t u32_parity;
	uint32_t u32_flow_control;
} USART;


/*
* Global vaiables
*/
GPIO g_st_usart2_rx_gpio;
GPIO g_st_usart2_tx_gpio;
USART g_st_usart2;


/*
*	Precondition:	fn_setup_clock()
*	Input:			None
*	Output:			None
*   Errors:         None
*	Impact:			changes gpio config, enables USART
*
* 	Description:
* 		This function setups a uart using the defines above
*/
void fn_setup_usart(USART *usart)	{
	/*setup the gpio needed*/
	fn_setup_gpio(usart->pst_gpio_tx);
	fn_setup_gpio(usart->pst_gpio_rx);

	/*Enables clock for usart*/
	switch ((uint32_t) usart->pst_usart_sel)	{
	case (uint32_t) USART1:
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
		break;
	case (uint32_t) USART2:
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
		break;
	case (uint32_t) USART3:
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
		break;
	case (uint32_t) UART4:
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART4);
		break;
	case (uint32_t) UART5:
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);
		break;
	case (uint32_t) LPUART1:
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);
		break;
	default:
		return;
		break;
	}

	/*deprecated*/
	// usart_set_baudrate(usart->u32_usart_sel, usart->u32_baud_rate);
	// usart_set_databits(usart->u32_usart_sel, usart->u32_data_bits);
	// usart_set_stopbits(usart->u32_usart_sel, usart->u32_stop_bits);
	// usart_set_mode(usart->u32_usart_sel, usart->u32_mode);
	// usart_set_parity(usart->u32_usart_sel, usart->u32_parity);
    // usart_set_flow_control(usart->u32_usart_sel, usart->u32_flow_control);


	LL_USART_Disable(usart->pst_usart_sel);
	// set baudrate
	if (usart->pst_usart_sel == USART1)	{
		LL_USART_SetBaudRate(usart->pst_usart_sel, rcc_apb1_frequency, LL_USART_OVERSAMPLING_16, usart->u32_baudrate);
	} else {
		LL_USART_SetBaudRate(usart->pst_usart_sel, rcc_apb2_frequency, LL_USART_OVERSAMPLING_16, usart->u32_baudrate);
	}

	LL_USART_SetDataWidth(usart->pst_usart_sel, usart->u32_data_bits);
	LL_USART_SetStopBitsLength(usart->pst_usart_sel, usart->u32_stop_bits);
	LL_USART_SetTransferDirection(usart->pst_usart_sel, usart->u32_mode);
	LL_USART_SetParity(usart->pst_usart_sel, usart->u32_parity);
	LL_USART_SetHWFlowCtrl(usart->pst_usart_sel, usart->u32_flow_control);
	LL_USART_Enable(usart->pst_usart_sel);

    // this came from an example provided by STM
	while((!(LL_USART_IsActiveFlag_TEACK(usart->pst_usart_sel))) || (!(LL_USART_IsActiveFlag_REACK(usart->pst_usart_sel)))); 
}


// code based off https://github.com/STMicroelectronics/STM32CubeL4/blob/master/Projects/NUCLEO-L476RG/Examples_LL/USART/USART_Communication_TxRx_DMA/Src/main.c
void fn_setup_usart_dma_rx(USART *usart, uint8_t *pu8_rx_buffer, uint16_t u32_buffer_size)	{
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

	LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_6, LL_DMA_DIRECTION_PERIPH_TO_MEMORY | LL_DMA_PRIORITY_HIGH | LL_DMA_MODE_NORMAL | LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT | LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_6, (uint32_t) &usart->pst_usart_sel->RDR, (uint32_t) pu8_rx_buffer, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_6));
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, u32_buffer_size);
	LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_6, LL_DMA_REQUEST_2);

	LL_USART_EnableDMAReq_RX(usart->pst_usart_sel);
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);
}

// returns how much space (in bytes) is left in the buffer
// subtract from length to get how much data is in the buffer
uint16_t fn_usart_disable_dma_rx(USART *usart)	{
	LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_6);
	LL_USART_DisableDMAReq_RX(usart->pst_usart_sel);
	return LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);
}

uint16_t fn_usart_get_dma_cntr_rx(USART *usart)	{
	return LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_6);
}

void fn_usart_reset_counter_dma_rx(USART *usart, uint16_t u32_buffer_size)	{
	LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_6, u32_buffer_size);
}

void fn_usart_enable_dma_rx(USART *usart)	{
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_6);
	LL_USART_EnableDMAReq_RX(usart->pst_usart_sel);
}





void fn_send_char(USART *pst_usart, char car_char)	{
	LL_USART_TransmitData8(pst_usart->pst_usart_sel, car_char);
	while (!LL_USART_IsActiveFlag_TXE(pst_usart->pst_usart_sel));
}

/**
*	@brief This function sends a char string over usart
* 
*	@param pst_usart pointer to usart struct
*	@param car_string  pointer to string to send
*
*
* 	@note Precondition:	fn_setup_usart()
* 	@note Impact: overwrites uart tx buffer
* 	@note Warning: This function could possibly cause an infinite loop if the uart is not setup correctly
*/
void fn_send_string(USART *pst_usart, const uint8_t *car_string)	{
	while (*car_string)	{
		LL_USART_TransmitData8(pst_usart->pst_usart_sel, *car_string);
		while (!LL_USART_IsActiveFlag_TXE(pst_usart->pst_usart_sel));
		car_string++;
	}
}

void fn_clear_rx_buffer(USART *pst_usart)	{
	LL_USART_ClearFlag_ORE(pst_usart->pst_usart_sel);
	LL_USART_ClearFlag_NE(pst_usart->pst_usart_sel);
	LL_USART_ClearFlag_FE(pst_usart->pst_usart_sel);
	LL_USART_ClearFlag_PE(pst_usart->pst_usart_sel);
	LL_USART_ReceiveData8(pst_usart->pst_usart_sel);		// empty the rx buffer
}

char fn_get_char(USART *pst_usart)	{		
	while (!LL_USART_IsActiveFlag_RXNE(pst_usart->pst_usart_sel));		// blocking
	return LL_USART_ReceiveData8(pst_usart->pst_usart_sel);
}

uint32_t fn_receive_string(USART *pst_usart, char *car_string, uint32_t u32_buff_size)	{
	for (uint32_t i = 0; i < u32_buff_size; i++)	{
		while (!LL_USART_IsActiveFlag_RXNE(pst_usart->pst_usart_sel));	// block until data is received - this is temporary till dma is implemented
		car_string[i] = LL_USART_ReceiveData8(pst_usart->pst_usart_sel);
		if (car_string[i] == '\n')	{
			car_string[i] = '\0';
			return i + 1;
		}
	}
	return 0;	// buffer overflow
}


__STATIC_INLINE
void SETUP_USART2(uint32_t u32_baudrate)	{
	g_st_usart2_tx_gpio = (GPIO) {
        .pst_port = USART2_TX_PORT,
        .u32_pin = USART2_TX_PIN,
        .u32_mode = LL_GPIO_MODE_ALTERNATE,
        .u32_up_down = LL_GPIO_PULL_NO,
        .u32_type = LL_GPIO_OUTPUT_PUSHPULL,
        .u32_speed = GPIO_SPEED_FREQ_HIGH,
        .u32_af = USART2_TX_AF
    };

    g_st_usart2_rx_gpio = (GPIO) {
        .pst_port = USART2_RX_PORT,
        .u32_pin = USART2_RX_PIN,
        .u32_mode = LL_GPIO_MODE_ALTERNATE,
        .u32_up_down = LL_GPIO_PULL_NO,
        .u32_type = LL_GPIO_OUTPUT_PUSHPULL,
        .u32_speed = GPIO_SPEED_FREQ_HIGH,
        .u32_af = USART2_RX_AF
    };

    // setup USART2
    g_st_usart2 = (USART) {
        .pst_gpio_tx = &g_st_usart2_tx_gpio,
        .pst_gpio_rx = &g_st_usart2_rx_gpio,
        .pst_usart_sel = USART2,
        .u32_baudrate = u32_baudrate,
        .u32_data_bits = LL_USART_DATAWIDTH_8B,
        .u32_stop_bits = LL_USART_STOPBITS_1,
        .u32_mode = LL_USART_DIRECTION_TX_RX,
        .u32_parity = LL_USART_PARITY_NONE,
        .u32_flow_control = LL_USART_HWCONTROL_NONE,
    };

    fn_setup_usart(&g_st_usart2);
}


__STATIC_INLINE
void USART2_SEND_STRING(const char *car_string)	{
	fn_send_string(&g_st_usart2, (uint8_t*)car_string);
}


__STATIC_INLINE
uint32_t USART2_RECEIVE_STRING(char *car_string, uint32_t u32_max_length)	{
	return fn_receive_string(&g_st_usart2, car_string, u32_max_length);
}

__STATIC_INLINE
void USART2_SEND_CHAR(char car_char)	{
	fn_send_char(&g_st_usart2, car_char);
}

__STATIC_INLINE
char USART2_RECEIVE_CHAR(void)	{
	return fn_get_char(&g_st_usart2);
}



__STATIC_INLINE
void USART2_CLEAR_RX_BUFFER(void)	{
	fn_clear_rx_buffer(&g_st_usart2);
}

#endif