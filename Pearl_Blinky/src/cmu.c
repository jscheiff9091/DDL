//***********************************************************************************
// Include files
//***********************************************************************************
#include "cmu.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void cmu_init(void){

		CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFXO);
		CMU_ClockEnable(cmuClock_HFPER, true);

		// By default, LFRCO is enabled
		CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);	// using LFXO or ULFRCO

		// Route LF clock to the LF clock tree

//LFXO/ULFRCO defined in main.h
#if (defined(LFXO) && defined(LETIMER)) || defined(LEUART)
		CMU_OscillatorEnable(cmuOsc_ULFRCO, false, false);
		CMU_OscillatorEnable(cmuOsc_LFXO, true, true);		//enable Low Frequency Crystal Oscilator
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); 	//select LFA Clock to be LFXO
#endif

#if defined(ULFRCO) && defined(LETIMER)
		/* If using ULFRCO Clock, use these 2 lines */
		CMU_OscillatorEnable(cmuOsc_ULFRCO, true, true);
		CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
#endif

#ifdef LETIMER
		CMU_ClockEnable(cmuClock_LFA, true);				//enable LFA Clock
		CMU_ClockEnable(cmuClock_LETIMER0, true);			//Enable clock to LETIMER0
#endif

#ifdef I2C
		CMU_ClockEnable(cmuClock_I2C0, true);				//Enable clock to I2C0
#endif

#ifdef LEUART
		CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);
		CMU_ClockEnable(cmuClock_LFB, true);
		CMU_ClockEnable(cmuClock_LEUART0, true);
#endif

		CMU_ClockEnable(cmuClock_CORELE, true);

		// Peripheral clocks enabled
		CMU_ClockEnable(cmuClock_GPIO, true);
		CMU_HFXOAutostartEnable(true, true, true);

}

