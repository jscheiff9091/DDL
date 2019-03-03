/*
 * leuart.h
 *
 *  Created on: Feb 26, 2019
 *      Author: Jacob S
 */

#ifndef SRC_LEUART_H_
#define SRC_LEUART_H_

#include "main.h"
#include "em_leuart.h"
#include "emu.h"
#include "queue.h"
#include "gpio.h"

/* Defines */
#define LEUART_DEFAULT		false
#define NOT_LAST_CHARACTER  false
#define LAST_CHARACTER		true


/* Type definitions */
typedef enum {
	TX_WAITING,
	TX_INPROGRESS
}Transmit_UART_State_t;

typedef struct {
	uint8_t * data;
	uint8_t index;
	Transmit_UART_State_t TX_state;
}Transmit_UART_Data_t;



/* Function Prototypes */

/*
 * Initialize LEUART peripheral, no inputs or outputs.
 */
void leuart_init();

/*
 * Send AT command to BLE module.
 * Input AT command character string.
 */
void leuart_send_AT_command(uint8_t * AT_command, uint8_t len_command);

/*
 * Initialize UART transmit struct.
 * Must pass in instance of the struct you are trying to initialize.
 */
void intialize_transmit_data_struct(Transmit_UART_Data_t * TX_struct);

/*
 * Send single byte to UART peripherals. If is_last_char asserted, RX is enabled and TX is disabled after transmission of the byte.
 * Inputs:
 * 		char_to_send is byte to be put in transmit buffer.
 * 		is_last_char is asserted if the char to be sent is the last in the stream.
 */
void UART_send_byte(uint8_t char_to_send, bool is_last_char);


#endif /* SRC_LEUART_H_ */
