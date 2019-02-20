/*
 * si7021.c
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */

#include "si7021.h"

/* The BEFORE, ACK, NACK, STOP, START, and BLOCK commands give instructions on how the next interrupt should handle the SDA line when
 * 		the interrupt after the sensor responds to the data byte being sent. BEFORE tells whether data is sent before or after the commands,
 * 		BLOCK delays commands being sent until the next in the interrupt, the rest send the command their name suggests. The commands for a
 * 		certain operation are queued in its assigned function, as seen in the functions below. The queue consists of 16 bit unsigned integers,
 * 		the MsB are the encoded commands described above, and the LsB is the data byte to be sent. These are masked and separated when read
 * 		from the queue in the interrupt handler.
 */


void write_UserRegister1(){
	clear_queue(&t_queue);
	enqueue(&t_queue,((BEFORE | ACK |STOP) << 8) | 0xE6);						//Data: Write USER REGISTER 1 command
	enqueue(&t_queue, 0x0001);													//Data: Set user register to 12 bit temp resolution

	response = BEFORE;
	counter = 0;
	I2C0->CMD |= I2C_CMD_CLEARTX;												//Clear TX Buffer and shift register
	I2C0->TXDATA = SI7021_WRITE;												//Write to SI7021 Temp Sensor
	I2C0->CMD |= I2C_CMD_START;													//Start Transmission
	for(int i = 0; i < 1000; i++);												//Give time for I2C state to be set
}

void read_UserRegister1(){
	clear_queue(&t_queue);
	enqueue(&t_queue, ((START) << 8) | 0xE7);									//Data: Read User Register 1 command
	enqueue(&t_queue, ((BEFORE | NACK | STOP | BLOCK) << 8) | 0x81);			//Data: Read from SI7021 temp sensor

	response = BEFORE;
	counter = 0;
	I2C0->CMD |= I2C_CMD_CLEARTX;
	I2C0->TXDATA = SI7021_WRITE;
	I2C0->CMD |= I2C_CMD_START;
}
