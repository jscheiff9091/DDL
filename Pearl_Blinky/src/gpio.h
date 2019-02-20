//***********************************************************************************
// Include files
//***********************************************************************************
#include "main.h"
#include "em_gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************

// LED0 pin is
#define	LED0_port			gpioPortF
#define LED0_pin			4
#define LED0_default		false 	// off
// LED1 pin is
#define LED1_port			gpioPortF
#define LED1_pin			5
#define LED1_default		false	// off

#define	I2C_SDA_PORT		gpioPortC
#define I2C_SDA_PIN			10
#define I2C_SCL_PORT		gpioPortC
#define	I2C_SCL_PIN			11
#define	SENS_ENABLE_PORT	gpioPortB
#define SENS_ENABLE_PIN		10

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_init(void);

