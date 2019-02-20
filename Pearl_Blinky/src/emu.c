/***************************************************************************//**
* @file
emu.c
*******************************************************************************
* @section License
* <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
*******************************************************************************
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
* obligation to support this Software. Silicon Labs is providing the
* Software "AS IS", with no express or implied warranties of any kind,
* including, but not limited to, any implied warranties of merchantability
* or fitness for any particular purpose or warranties against infringement
* of any proprietary rights of a third party.
*
* Silicon Labs will not be liable for any consequential, incidental, or
* special damages, or any other relief, or for any claim by any third party,
* arising from your use of this Software.
*
******************************************************************************/

#include "emu.h"

void blockSleepState(sleepstate_enum minMode)
{
	CORE_ATOMIC_IRQ_DISABLE();
	sleep_block_counter[minMode]++;
	CORE_ATOMIC_IRQ_ENABLE();
}

void unblockSleepState(sleepstate_enum minMode)
{
	CORE_ATOMIC_IRQ_DISABLE();
	if(sleep_block_counter[minMode] > 0)
	{
		sleep_block_counter[minMode]--;
	}
	CORE_ATOMIC_IRQ_ENABLE();
}

void sleep()
{
	if(sleep_block_counter[0] > 0)				//Must stay in EM0
	{
		return;
	}
	else if(sleep_block_counter[1] > 0)			//Must stay above EM1, stay at EM0
	{
		return;
	}
	else if(sleep_block_counter[2] > 0)			//Must stay above EM2, enter EM1
	{
		EMU_EnterEM1();
	}
	else if(sleep_block_counter[3] > 0)			//Must stay above EM3, enter EM2 (save registers and restore upon return)
	{
		EMU_EnterEM2(true);
	}
	else										//Nothing blocking EM3, enter EM3 (save registers and restore upon return)
	{
		EMU_EnterEM3(true);
	}
	return;
}


