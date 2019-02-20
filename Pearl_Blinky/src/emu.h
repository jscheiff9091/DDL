/*
 * emu.h
 *
 *  Created on: Jan 29, 2019
 *      Author: Jacob S
 */

#ifndef SRC_EMU_H_
#define SRC_EMU_H_

#include "main.h"
#include <stdbool.h>
#include "em_emu.h"
#include "em_core.h"

typedef enum{
	EM0,
	EM1,
	EM2,
	EM3,
	EM4
}sleepstate_enum;


void blockSleepState(sleepstate_enum minMode);

void unblockSleepState(sleepstate_enum minMode);

void sleep();



#endif /* SRC_EMU_H_ */
