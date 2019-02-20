/*
 * i2c.c
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */

#include "i2c.h"

void I2C_init(){
	/* Create and initialize i2c init object */
	I2C_Init_TypeDef i2c_init;

	i2c_init.enable = false;													//wait to enable I2C
	i2c_init.master = true;														//Make self as master
	i2c_init.freq = I2C_FREQ_FAST_MAX;											//Set clock frequency to 392157
	i2c_init.refFreq = 0;														//Use enabled clock source as reference
	i2c_init.clhr = i2cClockHLRAsymetric;

	I2C_Init(I2C0, &i2c_init);													//Initialize I2C0 with configured settings

	/* Interrupt Enable */
	I2C0->IEN |= I2C_IEN_NACK | I2C_IEN_ACK | I2C_IEN_RXDATAV;					//Set NACK, ACK, Receive data interrupts

	/* Set Pin Location */
	I2C0->ROUTELOC0 = I2C_ROUTELOC0_SDALOC_LOC15 | I2C_ROUTELOC0_SCLLOC_LOC15; 	//Route I2C to Sensor and expansion headers

	/* Enable SDA/SCL Pins*/
	I2C0->ROUTEPEN = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;

	/* Clear */
	I2C0->IFC = I2C_IFC_CLEARALL;												//Clear all pending interrupts
	I2C0->TXDATA = 0;															//Clear Transmit Buffer
	I2C0->CMD = I2C_CMD_ABORT;													//Clear any bus transactions taking place

	/* Final Initialization */
	NVIC_EnableIRQ(I2C0_IRQn);													//Enable I2C interrupts in NVIC
	I2C0->CTRL |= I2C_CTRL_EN;													//Enable I2C
	blockSleepState(EM2);														//Clock used cannot dip below EM1.

	write_UserRegister1();														//Write user register 1 to be 12 bit temp resolution

	while(I2C0->STATE & I2C_STATE_BUSY);

	read_UserRegister1();														//Read value of register for verification, stored in r_queue Global, value expected 0x3b or 0d59.
}

void I2C0_IRQHandler(void){
	uint32_t interrupt = I2C0->IF;
	if(interrupt & I2C_IF_NACK)
	{
		I2C0->CMD |= I2C_CMD_ABORT;
		for(int i = 0; i < 10; i++)
		{
			GPIO_PinOutClear(I2C_SCL_PORT, I2C_SCL_PIN);
			GPIO_PinOutSet(I2C_SCL_PORT, I2C_SCL_PIN);
		}
		I2C0->IFC |= I2C_IFC_NACK;
	}
	else if(interrupt & I2C_IF_RXDATAV)											//Handle Recieve data interrupt
	{
		uint16_t data1 = I2C0->RXDOUBLE;										//Read data buffer
		uint16_t data2 = I2C0->RXDOUBLE;										//Clear interrupt in Receive data buffer ( data read is garbage)
		enqueue(&r_queue, data1);												//Add data to recieve queue (0x3b or 59 decimal is successfully written to User Register)
		if(response & ACK)
		{
			I2C0->CMD |= I2C_CMD_ACK;											//Send ACK if commanded by response byte
		}
		if(response & NACK)
		{
			I2C0->CMD |= I2C_CMD_NACK;											//Send NACK if commanded by response byte
		}
		if(response & STOP)
		{
			I2C0->CMD |= I2C_CMD_STOP;											//Send STOP if commanded by response byte
			counter = 0;
		}
		if(response & START)
		{
			I2C0->CMD |= I2C_CMD_START;											//Send START if commanded by response byte
		}
	}
	else if(interrupt & I2C_IF_ACK)												//Handle ACK interrupts
	{
		I2C0->IFC |= I2C_IFC_ACK;
		if(response & BLOCK)													//Ignore interrupt if commanded by response byte
		{
			response &= ~BLOCK;
			return;
		}
		uint16_t n_transmission = t_queue.queue[counter];						//Set next transmission
		counter++;
		uint8_t n_response = (n_transmission & 0xFF00) >> 8;					//Get correct responses for next interrupt
		uint8_t n_data = n_transmission & 0xFF;									//Get next data byte
		if(response & BEFORE)													//Send data before ack/nack/start/stop if commanded by response byte
		{
			I2C0->TXDATA = n_data;
		}
		if(response & ACK)														//Send ACK if commanded by response byte
		{
			I2C0->CMD |= I2C_CMD_ACK;
		}
		if(response & NACK)														//Send NACK if commanded by response byte
		{
			I2C0->CMD |= I2C_CMD_NACK;
		}
		if(response & STOP)														//Send Stop if commanded by response byte
		{
			I2C0->CMD |= I2C_CMD_STOP;
		}
		if(response & START)													//send START if commanded by response byte
		{
			I2C0->CMD |= I2C_CMD_START;
		}
		if(!(response & BEFORE))												//Send data after ack/nack/start/stop if commanded by response byte
		{
			I2C0->TXDATA = n_data;
		}
		response = n_response;
	}
}
