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

/* Defines */
#define I2C_IFC_CLEARALL		0x7FFCF
#define BEFORE			0x80
#define ACK				0x04
#define NACK			0x08
#define STOP			0x02
#define START			0x01
#define BLOCK			0x40

/* Function Prototypes */

void I2C_init();

void I2C0_IRQHandler(void);





#endif /* SRC_I2C_H_ */
