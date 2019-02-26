/*
 * i2c.c
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */

#include "i2c.h"
#include "main.h"

extern volatile I2C_CMDPacket_t I2C_CMD_Read;
extern volatile I2C_CMDPacket_t I2C_CMD_Write;

void I2C_init(){
	initialize_CMDPacket(I2C_CMD_Read);
	initialize_CMDPacket(I2C_CMD_Write);

	/* Create and initialize i2c init object */
	I2C_Init_TypeDef i2c_init;

	i2c_init.enable = false;															//wait to enable I2C
	i2c_init.master = true;																//Make self as master
	i2c_init.freq = I2C_FREQ_FAST_MAX;													//Set clock frequency to 392157
	i2c_init.refFreq = 0;																//Use enabled clock source as reference
	i2c_init.clhr = i2cClockHLRAsymetric;

	I2C_Init(I2C0, &i2c_init);															//Initialize I2C0 with configured settings

	/* Interrupt Enable */
	I2C0->IEN |= I2C_IEN_NACK | I2C_IEN_ACK | I2C_IEN_RXDATAV;							//Set NACK, ACK, Receive data interrupts

	/* Set Pin Location */
	I2C0->ROUTELOC0 = I2C_ROUTELOC0_SDALOC_LOC15 | I2C_ROUTELOC0_SCLLOC_LOC15; 			//Route I2C to Sensor and expansion headers

	/* Enable SDA/SCL Pins*/
	I2C0->ROUTEPEN = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;

	/* Clear */
	I2C0->IFC = I2C_IFC_CLEARALL;														//Clear all pending interrupts
	I2C0->TXDATA = 0;																	//Clear Transmit Buffer
	if(I2C0->STATE & I2C_STATE_BUSY)
	{
		I2C0->CMD = I2C_CMD_ABORT;														//Clear any bus transactions taking place
	}

	/* Final Initialization */
	NVIC_EnableIRQ(I2C0_IRQn);															//Enable I2C interrupts in NVIC
	I2C0->CTRL |= I2C_CTRL_EN;															//Enable I2C
}

/*
 * Handle I2C interrupts. No inputs, sensor reading global varibale used as output.
 */
void I2C0_IRQHandler(void){
	uint32_t interrupt = I2C0->IF;
	if (I2C_CMD_Read.I2C_state == IDLE && sensor_reading != 0)
		return; //bitch
	if(interrupt & I2C_IF_NACK)															//Handle NACK interrupts
	{
		I2C0->CMD |= I2C_CMD_ABORT;														//ABORT I2C bus
		for(int i = 0; i < 10; i++)														//Reset sensors on the bus.
		{
			GPIO_PinOutClear(I2C_SCL_PORT, I2C_SCL_PIN);
			GPIO_PinOutSet(I2C_SCL_PORT, I2C_SCL_PIN);
		}
		I2C0->IFC |= I2C_IFC_NACK;														//Clear NACK interrupt flag
		death_star_plans_transmitted();
	}
	else if(interrupt & I2C_IF_ACK)
		{
			I2C0->IFC |= I2C_IFC_ACK;
			if(I2C_CMD_Read.I2C_state == ADDR_WRITE_SENT || I2C_CMD_Write.I2C_state == ADDR_WRITE_SENT)
			{
				if(I2C_CMD_Read.I2C_state == ADDR_WRITE_SENT)								//Address + write sent, expecting ACK, send command code
				{
					I2C0->TXDATA = (uint8_t)(I2C_CMD_Read.command & CMD_MASK);				//Update proper state (write or read depending on which command is being executed)
					I2C_CMD_Read.I2C_state = CMD_SENT;
				}
				else if(I2C_CMD_Write.I2C_state == ADDR_WRITE_SENT)
				{
					I2C_CMD_Write.I2C_state = CMD_SENT;
					I2C0->TXDATA = (uint8_t)(I2C_CMD_Write.command & CMD_MASK);
				}
			}
			else if(I2C_CMD_Read.I2C_state == CMD_SENT || I2C_CMD_Write.I2C_state == CMD_SENT)
			{
				if(I2C_CMD_Read.I2C_state == CMD_SENT)										//Command sent, expecting ACK, respond with start + Address + read for read operations
				{
					I2C0->CMD |= I2C_CMD_START;
					I2C0->TXDATA = SI7021_READ;
					I2C_CMD_Read.I2C_state = ADDR_READ_SENT;								//Set state to wait for data return
				}
				else if(I2C_CMD_Write.I2C_state == CMD_SENT)								//Command sent, expecting ACK, respond with data + ACK + STOP for write operations
				{
					I2C0->TXDATA = I2C_CMD_Write.write_data;
					I2C0->CMD |= I2C_CMD_ACK;
					I2C0->CMD |= I2C_CMD_STOP;
					I2C_CMD_Write.I2C_state = IDLE;											//Last interaction for write user register, set state to IDLE
					death_star_plans_transmitted();
				}
			}
			else if(I2C_CMD_Read.I2C_state == ADDR_READ_SENT)
			{
				return;
			}
		}
	else if(interrupt & I2C_IF_RXDATAV)													//Handle RXDATA interrupts
	{
		if(I2C_CMD_Read.I2C_state == ADDR_READ_SENT)									//Start bit + address + read sent
		{
			if((I2C_CMD_Read.command & CMD_MASK) == SI_READ_USER_REG)					//expecting single byte for Read User Register
			{
				sensor_reading = I2C0->RXDOUBLE;
				I2C0->CMD |= I2C_CMD_NACK;
				I2C0->CMD |= I2C_CMD_STOP;
				I2C_CMD_Read.I2C_state = IDLE;											//Last interaction with SI7021 for Read user reg, set state to IDLE
				death_star_plans_transmitted();
			}
			else if((I2C_CMD_Read.command & CMD_MASK) == SI_RHMEAS_HMM || (I2C_CMD_Read.command & CMD_MASK) == SI_TEMPMEAS_HMM)
			{
				sensor_reading = (I2C0->RXDOUBLE << SENSOR_READ_SHIFT);					//expecting MSB of 2 bytes returned for humidity and temperature readings
				I2C_CMD_Read.I2C_state = LAST_DATA_WAIT;								//Set state to wait for LSB
				I2C0->CMD |= I2C_CMD_ACK;
			}
		}
		else if(I2C_CMD_Read.I2C_state == LAST_DATA_WAIT)
		{
			sensor_reading |= I2C0->RXDOUBLE;											//Read LSB
			I2C_CMD_Read.I2C_state = IDLE;												//Last interaction for read temp and humidity set state to IDLE
			I2C0->CMD |= I2C_CMD_NACK;// | I2C_CMD_STOP;
			I2C0->CMD |= I2C_CMD_STOP;
			uint8_t temp_cels = get_temp(sensor_reading);
			if(temp_cels < SI7021_MIN_TEMP)
			{
				GPIO_PinOutSet(LED0_PORT, LED0_PIN);
			}
			else
			{
				GPIO_PinOutClear(LED0_PORT, LED0_PIN);
			}
			sensor_reading = 0;
			death_star_plans_transmitted();												//Disable LPM
		}
	}

}

void initialize_CMDPacket(I2C_CMDPacket_t I2C_CMDP)
{
	I2C_CMDP.I2C_state = IDLE;
	I2C_CMDP.command = 0;
	I2C_CMDP.write_data = 0;
}
