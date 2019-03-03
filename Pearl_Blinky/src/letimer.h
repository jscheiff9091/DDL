/* File: timer.h
 * Author: Jacob Scheiffler
 */

#ifndef SRC_LETIMER_H_
#define SRC_LETIMER_H_

#include "em_cmu.h"
#include "em_letimer.h"
#include "efm32pg12b500f1024gl125.h"
#include "main.h"
#include "gpio.h"
#include "emu.h"
#include <math.h>
#include "SI7021.h"

/*defines*/

/* User must set these 3 fields */
#define LED_PERIOD			5
#define	LED_ON_TIME			.0183

//LFXO/ULFRCO defined in main.h
#ifdef LFXO
#define CLOCK_FREQ          32768
#endif

#ifdef ULFRCO
#define CLOCK_FREQ			1000
#endif

/* LETIMER register macros */
#define TIMER_MAX_COUNT		65535									//Max Timer start value
#define LET_CTRL_COMP0TOP 	0x200									//Set COMP0TOP bit in CTRL register
#define LET_CTRL_FREE	  	0x1										//Clear free running REP0 mode in CTRL register
#define LET_COMP0			CLOCK_FREQ * LED_PERIOD					//Ticks for timer start value (before adjusting for clock divider)
#define LET_COMP1			CLOCK_FREQ * (LED_PERIOD - LED_ON_TIME)	//Ticks for COMP1 value (before adjusting for clock divider)
#define	LET_IF_UF			0x4										//Set/Check/Clear Underflow interrupt flag
#define	LET_IF_COMP1		0x2										//Set/Check/Clear COMP1 interrupt flag
#define LET_IF_COMP0        0x1										//COMP0 interrupt flag
#define LET_CLEARALL_INT	0x1F
#define LET_START			0x1										//Set start bit in CMD register

#define MULT_2				1

void letimer0_init(void);											//Sets clock divider, initializes LETIMER0, enables interrupts

void LETIMER0_IRQHandler(void);										//LETIMER0 interrupt handler

uint16_t clockDividerCalc();										//Calculate clock divider

#endif
