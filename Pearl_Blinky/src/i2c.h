/*
 * i2c.h
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include "main.h"
#include "em_i2c.h"
#include "emu.h"
#include "gpio.h"
#include "queue.h"
#include "SI7021.h"

/* Defines */
#define I2C_IFC_CLEARALL		0x7FFCF
#define I2C_WRITE				0
#define I2C_READ				1
#define CMD_MASK 				0xFF
#define	SENSOR_READ_SHIFT		8
#define NO_DATA_WRITE			0

/* Type Defs */
typedef enum {
	IDLE,
	ADDR_WRITE_SENT,
	CMD_SENT,
	ADDR_READ_SENT,
	LAST_DATA_WAIT
} I2C_State_t;

typedef struct {
	uint16_t command;
	uint8_t write_data;
	I2C_State_t I2C_state;
} I2C_CMDPacket_t;

/* Function Prototypes */

void I2C_init();

void I2C0_IRQHandler(void);

void initialize_CMDPacket(I2C_CMDPacket_t I2C_CMDP);



#endif /* SRC_I2C_H_ */
