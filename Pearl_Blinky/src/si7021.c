/*
 * si7021.c
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */

#include "si7021.h"

extern volatile I2C_CMDPacket_t I2C_CMD_Read;
extern volatile I2C_CMDPacket_t I2C_CMD_Write;

void i2c_send_read_command(uint8_t cmd_code, uint8_t only_big_data)
{
	I2C_CMD_Read.command = cmd_code;									//Set command for operation to be performed by SI7021
	I2C_CMD_Read.write_data = only_big_data;							//Set data to be written to SI7021 if any
	I2C_CMD_Read.I2C_state = ADDR_WRITE_SENT;							//Set state machine
	I2C0->CMD |= I2C_CMD_CLEARTX;										//Clear tx if any pending bytes need to be sent
	I2C0->TXDATA = SI7021_WRITE;										// Send slave address + write
	I2C0->CMD |= I2C_CMD_START;											//Send start bit
	for(int i = 0; i < 1000; i++);
}

void i2c_send_write_command(uint8_t cmd_code, uint8_t only_big_data)
{
	I2C_CMD_Write.command = cmd_code;									//set command code to send to Si7021
	I2C_CMD_Write.write_data = only_big_data;							//set data to write if any
	I2C_CMD_Write.I2C_state = ADDR_WRITE_SENT;							//Set state machine
	I2C0->CMD |= I2C_CMD_CLEARTX;										//Clear any TX buffer bytes waiting
	I2C0->TXDATA = SI7021_WRITE;										//Send Slave address + write
	I2C0->CMD |= I2C_CMD_START;											// Send start bit
	for(int i = 0; i < 1000; i++);
}

uint8_t get_temp(uint16_t sensor_return_val)
{
	uint8_t rv;
	rv = ((SI_MULTIPLY * sensor_return_val) / SI_DIVIDE) - SI_SUBTRACT;					//Equation given by SI7021 data sheet
	return rv;
}

void get_I2C_ready_for_transmission_of_death_star_plans()
{
	blockSleepState(EM2);
	if(I2C0->STATE & I2C_STATE_BUSY)
	{
		I2C0->CMD |= I2C_CMD_ABORT;
	}
	GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeWiredAnd, I2C_SDA_ON);			//SDA: Open drain out w/ Pullup resistor, set high initially
	GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeWiredAnd, I2C_SCL_ON);			//SCL: Open drain out w/ Pullup resistor, set high initially

	for (uint8_t i = 0; i < 9; i++) {
		GPIO_PinOutClear(I2C_SCL_PORT, I2C_SCL_PIN);
		GPIO_PinOutSet(I2C_SCL_PORT, I2C_SCL_PIN);
	}

}

void death_star_plans_transmitted()
{

	GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeDisabled, I2C_SCL_OFF);			//Disable SCL pin
	GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeDisabled, I2C_SDA_OFF);			//Disable SDA pin
	GPIO_PinOutClear(SENS_ENABLE_PORT, SENS_ENABLE_PIN);								//Clear sens enable
	unblockSleepState(EM2);																//unblock sleep mode 2
}
