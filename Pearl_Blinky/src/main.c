/***************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for SLSTK3402A
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "bsp.h"
#include "main.h"
#include "gpio.h"
#include "cmu.h"
#include "emu.h"
#include "letimer.h"
#include "i2c.h"
#include "si7021.h"
#include "queue.h"

volatile uint8_t sleep_block_counter[] = {0,0,0,0,0};
volatile uint8_t response;
volatile struct Transmit_Queue t_queue;
volatile uint8_t counter;
volatile struct Transmit_Queue r_queue;

int main(void)
{
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

  /* Chip errata */
  CHIP_Init();

  /* Init DCDC regulator and HFXO with kit specific parameters */
  EMU_EM23Init_TypeDef em23init = EMU_EM23INIT_DEFAULT;
  EMU_DCDCInit(&dcdcInit);
  em23init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
  EMU_EM23Init(&em23init);
  CMU_HFXOInit(&hfxoInit);

  /* Switch HFCLK to HFXO and disable HFRCO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

  /* Initialize Globals */
  initialize_queue(&t_queue);
  initialize_queue(&r_queue);

  /* Initialize clocks */
  cmu_init();

  /* Initialize GPIO */
  gpio_init();

  /* Initialize Timer */
#ifdef ULFRCO
  letimer0_init();
#endif
#ifdef LFXO
  letimer0_init();
#endif

  /* Initialize I2C */
  for(int i = 0; i < 1000000; i++);				//Give Sensor time to initialize

  I2C_init();


  for(int i = 0; i < 10; i++)
  {
	  GPIO_PinOutClear(I2C_SCL_PORT, I2C_SCL_PIN);
	  GPIO_PinOutSet(I2C_SCL_PORT, I2C_SCL_PIN);
  }

  /* Infinite blink loop */
  while (1) {
	  sleep();					//Adjust energy mode as necessary
  }
}
