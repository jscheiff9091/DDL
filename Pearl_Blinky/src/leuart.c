/*
 * leuart.c
 *
 *  Created on: Feb 26, 2019
 *      Author: Jacob Shyf
 */
#include "leuart.h"

extern volatile Transmit_UART_State_t TX_state;
extern volatile Receive_Buffer_t receive_buff;

void leuart_init()
{
	TX_state = TX_WAITING;
	initialize_queue(&receive_buff);

	LEUART_Init_TypeDef leuart_init;
	leuart_init.baudrate = 9600;						//Set Baud rate to 9600
	leuart_init.databits = leuartDatabits8;				//Set # data bits to 8
	leuart_init.enable = leuartDisable;					//Don't enable TX or RX
	leuart_init.parity = leuartNoParity;				//No parity bit
	leuart_init.refFreq = 0;							//Let init function set appropriate value for refFreq
	leuart_init.stopbits = leuartStopbits1;				//1 stop bit

	LEUART_Init(LEUART0, &leuart_init);

	LEUART0->CTRL |= LEUART_CTRL_LOOPBK;

	LEUART0->ROUTELOC0 |= LEUART_ROUTELOC0_RXLOC_LOC18 | LEUART_ROUTELOC0_TXLOC_LOC18;		//Location 18 = Expansion header 14 (RX) and 12 (TX)
	LEUART0->ROUTEPEN |= LEUART_ROUTEPEN_RXPEN | LEUART_ROUTEPEN_TXPEN;						//Enable TX and RX routing

	LEUART0->IEN |= LEUART_IEN_RXDATAV | LEUART_IEN_TXC;									//Enable TXC and RXDATAV interrupts
	LEUART0->IFC |= LEUART_IEN_TXC;
	NVIC_EnableIRQ(LEUART0_IRQn);															//Enable LEUART0 interrupts in NVIC
	blockSleepState(EM2);																	//Block sleep state 2 (using LFXO)

	/****   RESULT CAN BE SEEN IN receive_buff->queue *******/
	leuart_send_AT_command((uint8_t *) "AT+NAMEjasc", 11);
}

void leuart_send_AT_command(uint8_t * AT_command, uint8_t len_command)
{
	LEUART0->CMD |= LEUART_CMD_TXEN | LEUART_CMD_RXEN;										//Enable TX

	for(int i = 0; i < len_command; i++)
	{
		if(i == (len_command - 1)) {
			UART_send_byte(AT_command[i], LAST_CHARACTER);					//Send final character
			while(TX_state == TX_INPROGRESS);								//Wait until done transmitting (cleared in interrupt handler)
		}
		else {
			UART_send_byte(AT_command[i], NOT_LAST_CHARACTER);				//Send character
			while(TX_state == TX_INPROGRESS);								//Wait until character done transmitting
		}
	}
}

void UART_send_byte(uint8_t char_to_send, bool is_last_char)
{
	TX_state = TX_INPROGRESS;
	if(is_last_char) {
		uint16_t extended_char;
		extended_char = (uint16_t) char_to_send;												//Zero extend char to 16 bits
		LEUART0->TXDATAX |= LEUART_TXDATAX_TXDISAT | LEUART_TXDATAX_RXENAT | extended_char;		//Enable Receive/Disable Transmit after transmission,
	}
	else {
		LEUART0->TXDATA = char_to_send;															//Write char to
	}
}

void LEUART0_IRQHandler()
{
	uint16_t interrupt = LEUART0->IF;
	if(interrupt & LEUART_IF_RXDATAV)
	{
		uint8_t char_received = LEUART0->RXDATA;
		bool success = enqueue(&receive_buff, char_received);
		if(!success)
		{
			GPIO_PinOutSet(LED1_PORT, LED1_PIN);
		}
	}
	else if(interrupt & LEUART_IF_TXC)
	{
		LEUART0->IFC |= LEUART_IFC_TXC;
		TX_state = TX_WAITING;
	}
}

/*
void intialize_transmit_data_struct(Transmit_UART_Data_t * TX_struct)
{
	TX_struct->data = 0;
	TX_struct->index = 0;
	TX_struct->TX_state = TX_WAITING;
}
*/

