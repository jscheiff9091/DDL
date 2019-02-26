/*
 * si7021.h
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */

#ifndef SRC_SI7021_H_
#define SRC_SI7021_H_
#include "main.h"
#include "i2c.h"
#include "em_i2c.h"
#include "queue.h"

/* Defines */
#define SI7021_SLAVE_ADDR    	0x40
#define SI7021_WRITE			(SI7021_SLAVE_ADDR << 1) | I2C_WRITE	//0x80
#define SI7021_READ				(SI7021_SLAVE_ADDR << 1) | I2C_READ		//0x81
#define RES_12_8				0x01	//Temp resolution = 12 bit, RH resolution = 8 bit
#define RES_14_12				0x00	//Temp resolution = 14 bit, RH resolution = 12 bit
// define all Si7021 command codes.
#define SI_RESET            	0xFE
#define SI_RHMEAS_HMM        	0xE5    //Relative Humidity HMM == Hold master mode
#define SI_RHMEAS_NHMM        	0xF5	//Relative Humidity NHMM
#define SI_TEMPMEAS_HMM        	0xE3	//Temperature HMM
#define SI_TEMPMEAS_NHMM    	0xF3	//Temperature NHMM
#define SI_FAST_TEMPMEAS    	0xE0	//Read temp from last humidity reading
#define SI_WRITE_USER_REG    	0xE6	//Write user register
#define SI_READ_USER_REG     	0xE7	//Read user register

#define SI7021_MIN_TEMP			15		//Set minimum temperature for the SI7021

/* Numbers given to calculate temperature in Celsius */
#define	SI_MULTIPLY				175.72
#define SI_DIVIDE				65536
#define SI_SUBTRACT				46.65

/* Function Prototypes */
/* Send command that reads data from I2C.
 * cmd_code: command sent to I2C device.
 * only_big_data: data to be sent to I2C device (0 if no data needs to be passed)
 */
void i2c_send_read_command(uint8_t cmd_code, uint8_t only_big_data);

/* Send command that writes data from I2C.
 * cmd_code: command sent to I2C device.
 * only_big_data: data to be sent to I2C device (0 if no data needs to be passed)
 */
void i2c_send_write_command(uint8_t cmd_code, uint8_t only_big_data);

/*
 * Pass in temp reading returned from sensor and get out temperature in Celsius.
 * sensor_return_val: 16-bit output from SI7021 sensor for temperature reading
 */
uint8_t get_temp(uint16_t sensor_return_val);

/*
 * Enable SCL/SDA lines, enable I2C/I2C interrupts, block sleep state 1, abort any previous bus session
 * No Inputs
 */
void get_I2C_ready_for_transmission_of_death_star_plans();

/*
 * Disable I2C/I2C Interrupts, Unblock sleep state 1, Disable SCL/SDA lines, Disable Sensor Enable
 */
void death_star_plans_transmitted();

#endif /* SRC_SI7021_H_ */
