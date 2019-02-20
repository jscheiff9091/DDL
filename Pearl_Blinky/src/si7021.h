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
#define SI7021_READ 	0x81
#define SI7021_WRITE	0x80
#define BEFORE			0x80
#define ACK				0x04
#define NACK			0x08
#define STOP			0x02
#define START			0x01
#define BLOCK			0x40


/* Function Prototypes */
void write_UserRegister1();

void read_UserRegister1();


#endif /* SRC_SI7021_H_ */
