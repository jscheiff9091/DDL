//***********************************************************************************
// Include files
//***********************************************************************************
#include "gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************


//***********************************************************************************
// functions
//***********************************************************************************
void gpio_init(void){

	// Set LED ports to be standard output drive with default off (cleared)
	GPIO_DriveStrengthSet(LED0_PORT, gpioDriveStrengthStrongAlternateStrong);
//	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_PORT, LED0_PIN, gpioModePushPull, LED0_DEFAULT);

	GPIO_DriveStrengthSet(LED1_PORT, gpioDriveStrengthStrongAlternateStrong);
//	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_PORT, LED1_PIN, gpioModePushPull, LED1_DEFAULT);

#ifdef I2C
	/* Initialize I2C_SDA, I2C_SCL, SENSOR_ENABLE */
	GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeDisabled, I2C_SDA_OFF);					//SDA: Open drain out w/ Pullup resistor, set high initially
	GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeDisabled, I2C_SCL_OFF);					//SCL: Open drain out w/ Pullup resistor, set high initially

	GPIO_DriveStrengthSet(SENS_ENABLE_PORT, gpioDriveStrengthWeakAlternateWeak); 				//SENS: drive strength weak
	GPIO_PinModeSet(SENS_ENABLE_PORT, SENS_ENABLE_PIN, gpioModePushPull, SENS_ENABLE_OFF);		//SENS: Enable, Push-Pull Out Mode, set high
#endif
	for(int i = 0; i < 100000; i++);

}
