#include "letimer.h"


void letimer0_init(void)
{
	uint16_t clock_divider = clockDividerCalc();
	CMU_ClockDivSet(cmuClock_LETIMER0, clock_divider);

//LFXO/ULFRCO defined in main.h
#ifdef LFXO
	blockSleepState(EM3);							//Block EM3 if using LFXO
#endif

#ifdef ULFRCO
	blockSleepState(EM4);							//Block EM4 if using ULFRCO
#endif

	LETIMER0->CTRL |= LET_CTRL_COMP0TOP;			//COMP0 is top value
	LETIMER0->CTRL &= ~LET_CTRL_FREE;				//Set to free running mode
	LETIMER0->COMP0 = LET_COMP0 / clock_divider;	//Set COMP0 (top value)
	LETIMER0->COMP1 = LET_COMP1 / clock_divider;	//Set COMP1 (interrupt to turn on LED)
	LETIMER0->IEN |= LET_IF_UF | LET_IF_COMP1;		//Enable COMP1 and Underflow interrupts

	NVIC_EnableIRQ(LETIMER0_IRQn);					//Enable Low Energy Timer Interrupts
	LETIMER0->CMD |= LET_START;						//Start Low Energy Timer
}

void LETIMER0_IRQHandler(void)
{
	if(LETIMER0->IF & LET_IF_UF)
	{
		LETIMER0->IFC = LET_IF_UF;
		GPIO_PinOutClear(LED0_port, LED0_pin);
	}
	else if(LETIMER0->IF & LET_IF_COMP1)
	{
		LETIMER0->IFC = LET_IF_COMP1;
		GPIO_PinOutSet(LED0_port, LED0_pin);

	}
	else
	{
		LETIMER0->IFC = 0x1F;
	}
}

uint16_t clockDividerCalc()
{

	uint16_t clock_divider = 1;
	uint16_t clock_freq = CLOCK_FREQ;
	while(LED_PERIOD > (TIMER_MAX_COUNT / clock_freq))
	{
		clock_divider = clock_divider << 1;
		clock_freq = CLOCK_FREQ / clock_divider;
	}
	return clock_divider;
}
