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
	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
//	GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, LED0_default);

	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
//	GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
	GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, LED1_default);

#ifdef I2C
	/* Initialize I2C_SDA, I2C_SCL, SENSOR_ENABLE */
	GPIO_PinModeSet(I2C_SDA_PORT, I2C_SDA_PIN, gpioModeWiredAnd, 1);				//SDA: Open drain out w/ Pullup resistor, set high initially
	GPIO_PinModeSet(I2C_SCL_PORT, I2C_SCL_PIN, gpioModeWiredAnd, 1);				//SCL: Open drain out w/ Pullup resistor, set high initially

	GPIO_DriveStrengthSet(SENS_ENABLE_PORT, gpioDriveStrengthWeakAlternateWeak); 	//SENS: drive strength weak
	GPIO_PinModeSet(SENS_ENABLE_PORT, SENS_ENABLE_PIN, gpioModePushPull, true);		//SENS: Enable, Push-Pull Out Mode, set high
#endif

}
