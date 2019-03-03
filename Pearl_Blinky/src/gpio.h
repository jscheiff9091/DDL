//***********************************************************************************
// Include files
//***********************************************************************************
#include "main.h"
#include "em_gpio.h"

//***********************************************************************************
// defined files
//***********************************************************************************

// LED0 pin is
#define	LED0_PORT			gpioPortF
#define LED0_PIN			4
#define LED0_DEFAULT		false 	// off
// LED1 pin is
#define LED1_PORT			gpioPortF
#define LED1_PIN			5
#define LED1_DEFAULT		false	// off

#define	I2C_SDA_PORT		gpioPortC
#define I2C_SDA_PIN			10
#define I2C_SCL_ON			true
#define	I2C_SCL_OFF			true

#define I2C_SCL_PORT		gpioPortC
#define	I2C_SCL_PIN			11
#define	I2C_SDA_ON			true
#define I2C_SDA_OFF			true

#define	SENS_ENABLE_PORT	gpioPortB
#define SENS_ENABLE_PIN		10
#define SENS_ENABLE_ON		true
#define SENS_ENABLE_OFF		false

#define LEUART_RX_PORT		gpioPortD
#define LEUART_RX_PIN		11
#define LEUART_RX_DEFAULT	false
#define LEUART_TX_PORT		gpioPortD
#define	LEUART_TX_PIN		10
#define LEUART_TX_DEFAULT	true


//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_init(void);

